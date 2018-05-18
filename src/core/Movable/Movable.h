//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Util/Updatable.h>
#include <obj/NonCopy.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>

/* std */
#include <atomic>

using namespace mud; namespace toy
{
	TOY_CORE_EXPORT bool spin_2d(Entity& entity, const vec3& axis, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool lookat_2d(Entity& entity, const vec3& axis, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool move_2d(Entity& entity, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool move_2d(Movable& movable, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool steer_2d(Entity& entity, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool steer_2d(Movable& movable, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool stick_2d(Entity& entity, const vec3& target, float velocity, float time_step);
	TOY_CORE_EXPORT bool track_2d(Entity& entity, const vec3& target, float velocity, float time_step);

	class _refl_ TOY_CORE_EXPORT Movable : public NonCopy, public Updatable
	{
	public:
		_constr_ Movable(Entity& entity);
		~Movable();

		_attr_ Entity& m_entity;

		vec3 m_linear_velocity = Zero3;
		vec3 m_angular_velocity = Zero3;

		vec3 m_direction = Zero3;
		vec3 m_acceleration = Zero3;

		bool m_moving = false;
		vec3 m_previous_position;

		void setMovement(const vec3& position, const quat& rotation, const vec3& relLinVel, const vec3& relAngVel);

        void setLinearVelocity(const vec3& linVel);
		void modifyLinearVelocity(const vec3& linVel);

        void setAngularVelocity(const vec3& angVel);
		void modifyAngularVelocity(const vec3& angVel);

		void setAcceleration(const vec3& acceleration, const vec3& maxLinVel);
		void modifyAcceleration(const vec3& acceleration); 

        void next_frame(size_t tick, size_t delta);

	protected:

		vec3 m_maxLinearVelocity = Zero3;

		size_t m_movementUpdated = 0;
		size_t m_accelerationUpdated = 0;
		size_t m_lastTick = 0;
	};
}
