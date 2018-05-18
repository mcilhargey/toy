//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <math/Math.h>
#include <math/Vec.h>
#include <obj/Complex.h>
#include <core/Generated/Forward.h>
#include <core/Disq/Disq.h>
#include <obj/Util/Updatable.h>

/* std */
#include <vector>
#include <memory>
#include <list>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT MotionStrategy
	{
	public:
		virtual ~MotionStrategy() {}

		virtual void activate(vec3& position) = 0;
		virtual void adjustHeight(vec3& position) = 0;
		virtual void update(const vec3& oldPos, const vec3& pos, vec3& adjustedPos) = 0;
		virtual bool validity(const vec3& pos) = 0;
		virtual void nearestValid(vec3& destination, float margin) = 0;
	};
	typedef std::vector<Movable*> MovableVector;

	class _refl_ TOY_CORE_EXPORT Manoeuvrable : public NonCopy
	{
	public:
		_constr_ Manoeuvrable(Entity& entity, float radius /*= 0.3f*/, float height /*= 2.f*/);
		~Manoeuvrable();

	public:
		 _attr_ Entity& m_entity;
		//Movable& m_movable;

		 unique_ptr<Pathfinder> m_pathfinder;

#ifdef TOY_DISQS
		Disq m_disq;
		Disq m_avoidDisq;
#endif

		_attr_ _mut_ float m_radius;
		_attr_ _mut_ float m_height;

		void setMotionStrategy(object_ptr<MotionStrategy> strategy);

		void next_frame(size_t tick, size_t delta);

		void setPosition(const vec3& pos);

		void steer();

	public:
		void destination(vec3& destination);
		void adjustHeight(vec3& position);

		void computePath(const vec3& destination);
		void updatePath();

		vec3 detour(Manoeuvrable& manoeuvrable, Atoll* atoll);
		void avoid(const vec3& spot);

		bool forecast(const vec3& spot);
		bool forecastNavmesh(const vec3& spot);

		object_ptr<DetourPath> m_path;
		vec3 m_waypoint;

		bool popWaypoint();

	protected:
		Entity* m_avoiding = nullptr;

		object_ptr<MotionStrategy> m_motionStrategy;
	};
}
