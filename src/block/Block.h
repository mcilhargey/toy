//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Proto.h>
#include <math/Vec.h>
#include <math/Grid.h>
#include <core/Entity/Entity.h>
#include <core/Physic/Scope.h>
#include <block/Generated/Forward.h>

/* std */
#include <vector>
#include <memory>

namespace mud
{
	template struct _refl_ _struct_ TOY_BLOCK_EXPORT Grid<toy::Block*>;
}

using namespace mud; namespace toy
{
	struct Hunk
	{
		Block* block = nullptr;
		size_t index = SIZE_MAX;
		Element* element = nullptr;

		operator bool() { return block != nullptr; }

		Hunk(Block& b, size_t i, Element* e) : block(&b), index(i), element(e) {}
		Hunk() {}
	};

	class _refl_ TOY_BLOCK_EXPORT Block
	{
	public:
		_constr_ Block(Entity& entity, Emitter& emitter, Block* parentblock, size_t index, float size);

		_attr_ Entity& m_entity;
		_attr_ Emitter& m_emitter;
		_attr_ _link_ Block* m_parentblock;
		_attr_ size_t m_index;
		_attr_ float m_size;
		_attr_ size_t m_updated = 0;

		size_t m_depth = 0;

		bool m_subdived = false;

		Grid<Element*> m_chunks;
		Grid<Block*> m_subblocks;

		Block* m_neighbours[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		_meth_ void subdivide();

		_meth_ void reset();
		_meth_ void chunk(size_t x, size_t y, size_t z, Element& element);
		_meth_ void commit();

		void subdivideTo(size_t depth);

		size_t depth();

		vec3 min();
		vec3 max();
		vec3 coordinates();

		size_t subdiv();
		float chunkSize();

		Sector& sector();

		vec3 localBlockCoordinates(size_t index);
		vec3 localBlockCoordinates(Block& child);
		vec3 blockCoordinates(Block& child);

		vec3 localChunkCoordinates(size_t index);
		vec3 chunkCoordinates(size_t index);

		vec3 chunkPosition(size_t index);

		Hunk hunkAt(size_t index);

		Hunk neighbour(size_t index, Side side);
		Hunk neighbour(Hunk& hunk, Side side);

	protected:
		EmitterScope& m_scope;
	};

	class _refl_ TOY_BLOCK_EXPORT OBlock : public Construct
	{
	public:
		_constr_ OBlock(Id id, Entity& parent, const vec3& position, Block* parentblock, size_t index, float size);

		_comp_ _attr_ Entity m_entity;
		_comp_ Emitter m_emitter;
		_comp_ Block m_block;
	};
}
