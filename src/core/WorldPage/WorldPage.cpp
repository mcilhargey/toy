//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/WorldPage/WorldPage.h>

#include <geom/Shapes.h>
#include <geom/Mesh.h>
#include <geom/Geom.h>

#include <core/Entity/Entity.h>
#include <core/Movable/Movable.h>
#include <core/Physic/Scope.h>

#include <core/World/World.h>
#include <core/World/Section.h>
#include <core/Physic/PhysicWorld.h>

#include <core/Physic/Collider.h>
#include <core/Physic/Solid.h>

using namespace mud; namespace toy
{
	WorldMedium::WorldMedium()
		: Medium("World", false)
	{}

	WorldPage::WorldPage(Entity& entity, Emitter& emitter, bool open, const vec3& extents)
        : m_entity(entity)
		, m_emitter(emitter)
		, m_open(open)
		, m_extents(extents)
		, m_world(entity.m_world)
		, m_scope(emitter.addScope(WorldMedium::me(), Cube(m_extents / 2.f), CM_SOURCE))
		, m_geom(make_object<Geometry>())
    {
		entity.m_world.addTask(this, short(Task::World)); // @todo in the long term this should be moved out of the entity's responsibility
		m_entity.m_contents.observe(*this);
	}

    WorldPage::~WorldPage()
    {
		m_entity.m_world.removeTask(this, short(Task::World)); // @todo in the long term this should be moved out of the entity's responsibility
		m_entity.m_contents.unobserve(*this);
	}

	void WorldPage::next_frame(size_t tick, size_t delta)
	{
		UNUSED(tick); UNUSED(delta);
		if(m_updated > m_last_rebuilt)
			this->buildGeometry();
	}

	void WorldPage::buildGeometry()
	{
		if(m_build_geometry)
		{
			printf("INFO: Rebuilding WorldPage geometry\n");

			m_build_geometry(*this);
			this->updateGeometry();
			m_last_rebuilt = m_entity.m_lastTick;

			printf("INFO: Rebuilt WorldPage geometry, %zu vertices\n", m_geom->m_vertices.size());
		}
	}

	void WorldPage::updateGeometry()
	{
		if(m_geom->m_triangles.size() > 0)
			m_ground = make_object<Solid>(m_entity, *this, *m_geom, SolidMedium::me(), CM_GROUND, true);
	}

	void WorldPage::addContact(Collider& object)
	{
		UNUSED(object);
		// these are contacts from the world geometry, so that's not the right place to switch worldpage of an entity
		//object.m_entity.setParent(m_entity);
	}

	void WorldPage::removeContact(Collider& object)
	{
		UNUSED(object);
		//object.m_entity.setParent(*m_entity.m_parent);
	}

	void WorldPage::handleAdd(Entity& entity)
	{
		UNUSED(entity);
		//if(!entity.isa<Movable>())
		//	m_updated = m_entity.m_lastTick;
	}

	void WorldPage::handleRemove(Entity& entity)
	{
		UNUSED(entity);
		//if(!entity.isa<Movable>())
		//	m_updated = m_entity.m_lastTick;
	}

	void WorldPage::groundPoint(const vec3& position, bool relative, vec3& groundPoint)
	{
		// to absolute
		vec3 start(position.x, -m_extents.y / 2, position.z);
		vec3 end(position.x, +m_extents.y / 2, position.z);

		if(relative)
		{
			start += m_entity.m_position;
			end += m_entity.m_position;
		}

		Ray ray = { start, end, normalize(end - start), normalize(start - end) };
		groundPoint = m_world.part<PhysicWorld>().groundPoint(ray) - m_entity.m_position;

		if(any(isnan(groundPoint)) || any(isinf(groundPoint)))
			printf("ERROR: raycast ground point failed, position result invalid\n");
	}

	void WorldPage::raycastGround(const vec3& start, const vec3& end, vec3& groundPoint)
	{
		Ray ray = { start, end, normalize(end - start), normalize(start - end) };
		groundPoint = m_world.part<PhysicWorld>().groundPoint(ray);
	}
}
