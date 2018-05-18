//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <block/Generated/Types.h>
#include <block/Block.h>

#include <geom/Shapes.h>

#include <core/Entity/Entity.h>

#include <core/Physic/Scope.h>
#include <core/Physic/CollisionShape.h>

#include <core/WorldPage/WorldPage.h>

#include <block/Sector.h>

#define BLOCK_SUBDIV 20U

using namespace mud; namespace toy
{
	void index_blocks(const ivec3& grid_size, Grid<Block*>& grid, const std::vector<Block*>& blocks, const std::vector<Sector*>& sectors)
	{
		grid.reset(grid_size.x, grid_size.y, grid_size.z);

		for(size_t i = 0; i < blocks.size(); ++i)
		{
			ivec3 coord = sectors[i]->m_coordinate;
			grid.gridAt(coord.x, coord.y, coord.z) = blocks[i];
		}

		for(size_t i = 0; i < blocks.size(); ++i)
		{
			ivec3 coord = sectors[i]->m_coordinate;
			size_t index = grid.indexAt(coord.x, coord.y, coord.z);
			for(Side side : Sides)
				blocks[i]->m_neighbours[side] = grid.border(index, side) ? nullptr : grid.neighbourItem(index, side);
		}
	}

	Block::Block(Entity& entity, Emitter& emitter, Block* parentblock, size_t index, float size)
		: m_entity(entity)
		, m_emitter(emitter)
		, m_parentblock(parentblock)
		, m_index(index)
		, m_size(size)
		, m_chunks(BLOCK_SUBDIV)
		, m_subblocks(2)
		, m_scope(m_emitter.addScope(WorldMedium::me(), Cube(m_size), CM_SOURCE))
	{}

	size_t Block::depth()
	{
		return m_parentblock ? m_parentblock->depth() + 1 : 0;
	}

	vec3 Block::min()
	{
		float halfSize = m_size / 2.f;
		return m_entity.absolutePosition() - vec3(halfSize, halfSize, halfSize);
	}

	vec3 Block::max()
	{
		float halfSize = m_size / 2.f;
		return m_entity.absolutePosition() + vec3(halfSize, halfSize, halfSize);
	}

	vec3 Block::coordinates()
	{
		return m_parentblock ? m_parentblock->blockCoordinates(*this) : vec3(0, 0, 0);
	}

	size_t Block::subdiv()
	{
		return BLOCK_SUBDIV;
	}

	float Block::chunkSize()
	{
		return m_size / this->subdiv();
	}

	void Block::reset()
	{
		for(Element*& chunk : m_chunks)
			chunk = nullptr;
	}

	void Block::chunk(size_t x, size_t y, size_t z, Element& element)
	{
		m_chunks.gridAt(x, y, z) = &element;
	}

	void Block::commit()
	{
		m_updated++;
		this->sector().m_worldPage.m_updated++; // = m_entity.m_lastTick;
	}

	vec3 Block::localBlockCoordinates(Block& child)
	{
		size_t index = child.m_index;
		return this->localBlockCoordinates(index);
	}

	vec3 Block::localBlockCoordinates(size_t index)
	{
		return vec3(m_subblocks.x(index), m_subblocks.y(index), m_subblocks.z(index));
	}

	vec3 Block::blockCoordinates(Block& subblock)
	{
		vec3 coordinates = this->localBlockCoordinates(subblock);

		if(m_parentblock)
			coordinates += m_parentblock->blockCoordinates(*this) * 2.f;

		return coordinates;
	}

	vec3 Block::localChunkCoordinates(size_t index)
	{
		return vec3(m_chunks.x(index), m_chunks.y(index), m_chunks.z(index));
	}

	vec3 Block::chunkCoordinates(size_t index)
	{
		vec3 coordinates = this->localChunkCoordinates(index);

		if(m_parentblock)
			coordinates += m_parentblock->blockCoordinates(*this) * float(this->subdiv());

		return coordinates;
	}

	vec3 Block::chunkPosition(size_t index)
	{
		vec3 coordinates = this->localChunkCoordinates(index) * this->chunkSize() + this->chunkSize() / 2.f - m_size / 2.f;
		return coordinates;
	}

	Hunk Block::hunkAt(size_t index)
	{
		return Hunk(*this, index, m_chunks[index]);
	}

	void Block::subdivide()
	{
		for(size_t index = 0; index < 8; ++index)
		{
			float halfSize = m_size / 2.f;
			float halfSubdivSize = halfSize / 2.f;
			vec3 position = this->localBlockCoordinates(index) * halfSize - halfSize + halfSubdivSize;

			Block& block = m_entity.constructNested<OBlock>(position, this, index, m_size / 2).part<Block>();

			m_subblocks.push_back(&block);
		}

		m_subdived = true;
		// "update" trick
		m_entity.setPosition(m_entity.m_position);
	}

	void Block::subdivideTo(size_t depth)
	{
		this->subdivide();

		if(depth - 1 > 0)
		{
			for(Block* block : m_subblocks)
				block->subdivideTo(depth - 1);
		}
	}

	Hunk Block::neighbour(size_t index, Side side)
	{
		if(m_chunks.border(index, side))
		{
			if(!m_neighbours[side])
				return Hunk();

			size_t neighbour = m_neighbours[side]->m_chunks.neighbourMod(index, side);
			return m_neighbours[side]->hunkAt(neighbour);
		}
		else
		{
			size_t neighbour = m_chunks.neighbour(index, side);
			return this->hunkAt(neighbour);
		}
	}

	Hunk Block::neighbour(Hunk& hunk, Side side)
	{
		return this->neighbour(hunk.index, side);
	}

	Sector& Block::sector()
	{
		Entity* parent = m_entity.m_parent;
		for(size_t depth = 1; !parent->has_part(type<Sector>()); parent = parent->m_parent)
			++depth;

		return parent->part<Sector>();
	}

	OBlock::OBlock(Id id, Entity& parent, const vec3& position, Block* parentblock, size_t index, float size)
		: Construct(m_entity, proto<OBlock>())
		, m_entity(id, proto<OBlock>(), parent, position, ZeroQuat)
		, m_emitter(m_entity)
		, m_block(m_entity, m_emitter, parentblock, index, size)
	{
		// @5603 : adding to nested only when object is finish -> in prototype
		this->index(m_emitter, m_block, *this);
		m_entity.m_parent->m_contents.add(m_entity);
	}
}
