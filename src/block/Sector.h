//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#pragma once

/* toy */
#include <obj/Proto.h>
#include <core/WorldPage/WorldPage.h>
#include <core/WorldPage/BufferPage.h>
#include <core/Navmesh/Navmesh.h>
#include <block/Generated/Forward.h>
#include <block/Element.h>

/* std */
#include <vector>

using namespace mud; namespace toy
{
	typedef std::vector<Chunk*> ChunkVector;

	class _refl_ TOY_BLOCK_EXPORT Sector : public Construct
	{
	public:
		_constr_ Sector(Id id, Entity& parent, const vec3& position, const uvec3& coordinate, float size);

		_comp_ _attr_ Entity m_entity;
		_comp_ Emitter m_emitter;
		_comp_ WorldPage m_worldPage;
		_comp_ BufferPage m_bufferPage;
		_comp_ Navblock m_navblock;

		_attr_ uvec3 m_coordinate;
		_attr_ float m_size;

		_attr_ _mut_ Block* m_block;

		std::vector<Heap*> m_heaps;
	};

	TOY_BLOCK_EXPORT _func_ void index_blocks(const ivec3& grid_size, Grid<Block*>& grid, const std::vector<Block*>& blocks, const std::vector<Sector*>& sectors);
}
