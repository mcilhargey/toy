//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <block/Generated/Types.h>
#include <block/Sector.h>

#include <core/Entity/Entity.h>
#include <core/World/World.h>

#include <block/Element.h>
#include <block/Block.h>

using namespace mud; namespace toy
{
	Sector::Sector(Id id, Entity& parent, const vec3& position, const uvec3& coordinate, float size)
		: Construct(m_entity, proto<Sector>())
		, m_entity(id, proto<Sector>(), parent, position, ZeroQuat)
		, m_emitter(m_entity)
		, m_worldPage(m_entity, m_emitter, true, vec3(size, size, size))
		, m_bufferPage(m_entity, m_worldPage, true)
		, m_navblock(m_entity.m_world.part<Navmesh>(), m_entity, m_worldPage)
		, m_coordinate(coordinate)
		, m_size(size)
		, m_block(nullptr)
		, m_heaps()
	{
		// @5603 : adding to nested only when object is finish -> in prototype
		this->index(m_emitter, m_worldPage, m_bufferPage, m_navblock, *this);
		m_entity.m_parent->m_contents.add(m_entity);
	}
}
