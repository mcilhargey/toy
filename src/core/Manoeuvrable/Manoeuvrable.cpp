//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Generated/Types.h>
#include <core/Manoeuvrable/Manoeuvrable.h>

#include <geom/Intersect.h>

#include <core/World/World.h>
#include <core/Entity/Entity.h>
#include <core/Entity/Entity.h>
#include <core/Movable/Movable.h>
#include <core/Physic/Collider.h>
#include <core/Physic/CollisionShape.h>
#include <core/Physic/Solid.h>
#include <core/WorldPage/WorldPage.h>

#include <core/Manoeuvrable/GroundMotion.h>
#include <core/Path/DetourPath.h>
#include <core/Path/Pathfinder.h>

#include <core/World/Section.h>

#include <algorithm>

#define CONTACT_THRESHOLD 0.1f

using namespace mud; namespace toy
{
	Manoeuvrable::Manoeuvrable(Entity& entity, float radius, float height)
		: m_entity(entity)
		//, m_movable(entity.part<Movable>())
#ifdef TOY_DISQS
		, m_disq(entity, radius, height, SolidMedium::me(), CM_SOLID)
		, m_avoidDisq(entity, radius * 3.f, height, SolidMedium::me(), CM_SOLID)
#endif
		, m_radius(radius)
		, m_height(height)
		, m_motionStrategy(make_object<GroundMotion>(m_entity))
	{
		//m_motionStrategy->activate(m_entity.m_position);

#ifdef TOY_DISQS
		m_disq.imbed();
#endif
	}

	Manoeuvrable::~Manoeuvrable()
	{
#ifdef TOY_DISQS
		m_disq.unhooked();
#endif
	}

	void Manoeuvrable::destination(vec3& destination)
	{
		m_motionStrategy->nearestValid(destination, 2.f);
	}

	void Manoeuvrable::setPosition(const vec3& position)
	{
		m_entity.setPosition(position);
		this->adjustHeight(m_entity.m_position);
	}

	void Manoeuvrable::adjustHeight(vec3& position)
	{
		m_motionStrategy->adjustHeight(position);
	}

	void Manoeuvrable::computePath(const vec3& destination)
	{
		Navmesh& navmesh = m_entity.m_world.part<Navmesh>();
		if(!m_pathfinder)
			m_pathfinder = make_unique<Pathfinder>(navmesh);

		m_path = make_object<DetourPath>(m_entity, *m_pathfinder, m_entity.absolutePosition(), destination);
		m_path->compute();
	}

	void Manoeuvrable::updatePath()
	{
		m_path->compute();
	}

	bool Manoeuvrable::popWaypoint()
	{
		if(m_path->m_waypoints.empty())
		{
			m_path = nullptr;
			return false;
		}

		m_waypoint = vector_pop(m_path->m_waypoints);
		//this->adjustHeight(m_waypoint);

		return true;
	}

	bool Manoeuvrable::forecast(const vec3& spot)
	{
		if(!forecastNavmesh(spot))
			return false;
#ifdef TOY_DISQS
		if(m_disq.collisions(spot))
			return false;
#endif

		return true;
	}
	
	bool Manoeuvrable::forecastNavmesh(const vec3& pos)
	{
		return m_motionStrategy->validity(pos);
	}

	vec3 Manoeuvrable::detour(Manoeuvrable& target, Atoll* island)
	{
		vec3 invdirection = m_entity.m_position - island->m_center;
		vec3 direction = island->m_center - m_entity.m_position;
		
		
		// We take three points, two tangents and one straight from the island radius
		float tangent = sqrt(length2(direction) - (island->totalRadius() * island->totalRadius()));
		vec3 p1;
		vec3 p2 = island->m_center + (normalize(invdirection) * (island->m_farthestDist + island->m_highRadius));
		vec3 p3;
		circle_circle_intersection(m_entity.m_position, tangent, island->m_center, island->totalRadius(), p1, p3);

		float d1 = distance2(p1, target.m_entity.m_position);
		float d2 = distance2(p2, target.m_entity.m_position);
		float d3 = distance2(p3, target.m_entity.m_position);

		if(d1 < d2 && d1 < d3)
			return p1;
		else if(d2 < d3)
			return p2;
		else
			return p3;
	}

	/*void Manoeuvrable::stick(Manoeuvrable* sticker)
	{
		sticker->m_entity.setPosition(this->spot(sticker));
	}*/

	void Manoeuvrable::setMotionStrategy(object_ptr<MotionStrategy> strategy)
	{
		m_motionStrategy = std::move(strategy);
	}

	void Manoeuvrable::next_frame(size_t tick, size_t delta)
	{
		UNUSED(tick); UNUSED(delta);
#ifdef TOY_DISQS
		m_disq.next_frame(tick, delta);
#endif

		this->steer();

		//if(m_motionStrategy)
		//	m_motionStrategy->update(m_movable.m_oldPosition, m_entity.m_position, m_entity.m_position);
	}

	void Manoeuvrable::steer()
	{
		/*for(Contact& contact : m_avoidDisq.contacts())
		{
			if(!m_avoiding)
			{
			}

			if(&contact.disq->m_entity != m_avoiding)
			{ }
			// If the contact is
		}*/
	}

	/*void Manoeuvrable::addedToWorld(World* world)
	{
		m_motionStrategy.resetGroundMotion(m_entity);
		m_motionStrategy->activate(*m_entity.m_position);
		
		m_entity.setPosition(this->closestFreeSpot(m_entity.m_position));
	}

	void Manoeuvrable::removedFromWorld(World* world)
	{
		m_motionStrategy.reset();
	}*/
}
