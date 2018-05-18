//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
/* toy */

#include <core/Movable/Movable.h>

#include <math/VecOps.h>

#define TOY_PRIVATE
#include <core/Bullet.h>

#include <core/World/World.h>
#include <core/Entity/Entity.h>
#include <core/Movable/MotionState.h>

#include <core/World/Section.h>

using namespace mud; namespace toy
{
	float angle_flat(const vec3& v1, const vec3& v2)
	{
		vec3 vec1 = normalize(vec3{ v1.x, 0.f, v1.z });
		vec3 vec2 = normalize(vec3{ v2.x, 0.f, v2.z });

		return oriented_angle(vec1, vec2, Y3);
	}

	bool spin_2d(Entity& entity, const vec3& axis, const vec3& target, float velocity, float time_step)
	{
		UNUSED(time_step);
		vec3 direction = normalize(target - entity.m_position);
		float total_angle = angle_flat(entity.front(), direction);
		float angle = min(sign(total_angle) * velocity, total_angle);
		quat rotation = rotate(entity.m_rotation, angle, axis);
		entity.setRotation(rotation);
		return angle == total_angle;
	}

	bool lookat_2d(Entity& entity, const vec3& axis, const vec3& target, float velocity, float time_step)
	{
		UNUSED(axis); UNUSED(velocity); UNUSED(time_step);
		vec3 direction = normalize(target - entity.m_position);
		float targetAngle = angle_flat(-Z3, direction);
		quat rotation = angle_axis(targetAngle, Y3);
		entity.setRotation(rotation);
		return true;
	}

	vec3 project_move_2d(Entity& entity, const vec3& target, float velocity, float time_step)
	{
		vec3 direction = normalize(target - entity.m_position);
		direction.y = 0.f;

		vec3 movement = direction * velocity * time_step;
		vec3 distance = target - entity.m_position;
		distance.y = 0.f;

		if(length2(movement) >= length2(distance))
			return target;
		else
			return entity.m_position + movement;
	}

	bool move_2d(Entity& entity, const vec3& target, float velocity, float time_step)
	{
		if(target == entity.m_position)
			return true;

		vec3 position = project_move_2d(entity, target, velocity, time_step);
		entity.setPosition(position);
		return position == target;
	}

	bool move_2d(Movable& movable, const vec3& target, float velocity, float time_step)
	{
		UNUSED(time_step);
		if(target == movable.m_entity.m_position)
			return true;
		movable.setLinearVelocity(normalize(target - movable.m_entity.m_position) * velocity);
		return false;
	}

	bool steer_2d(Movable& movable, const vec3& target, float velocity, float time_step)
	{
		UNUSED(velocity); UNUSED(time_step);
		vec3 segment = target - movable.m_entity.absolutePosition();
		segment.y = 0.f;

		if(all(less(abs(segment), Unit3 * 0.001f)))
			return true;

		const float max_speed = 1.f;
		const float seek_force = 0.5f;
		const float approach_radius = 0.35f;

		vec3 desired = length(segment) < approach_radius ? segment / approach_radius * max_speed
														 : normalize(segment) * max_speed;
		//vec3 desired = normalize(segment) * max_speed;
		vec3 steer = desired - movable.m_direction;
		if(length(steer) > seek_force)
			steer *= seek_force / length(steer);
		movable.m_acceleration = steer;

		movable.m_direction += steer;

		if(length(movable.m_direction) > max_speed)
			movable.m_direction *= max_speed / length(movable.m_direction);

		float angle = oriented_angle(movable.m_direction, -Z3, Y3);
		movable.m_entity.m_rotation = rotate(ZeroQuat, -angle, Y3);
		movable.m_linear_velocity = rotate(movable.m_direction, angle, Y3);

		return false;
	}

	bool steer_2d(Entity& entity, const vec3& target, float velocity, float time_step)
	{
		bool done = spin_2d(entity, Y3, target, 1.f, time_step);
		if(true) // m_agent.forecast(project_move_2d(entity, target, velocity, time_step))
			done &= move_2d(entity, target, velocity, time_step);
		return done;
	}

	bool stick_2d(Entity& entity, const vec3& target, float velocity, float time_step)
	{
		//if(m_target.moving())
		//	m_target.stick(m_source);

		spin_2d(entity, Y3, target, 1.f, time_step);
		move_2d(entity, target, velocity, time_step);
		return false;
	}

	bool track_2d(Entity& entity, const vec3& target, float velocity, float time_step)
	{
		/*if(m_targetMovable && m_targetMovable->m_moving)
		{
			if(m_targetManoeuv)
				m_sticky.setPosition(m_targetManoeuv->m_disq.closest(m_agent.m_disq));
			else
				m_sticky.setPosition(m_agent.m_disq.attract(m_agent.m_entity.m_position, m_target.m_position, m_distance));
		}*/

		return steer_2d(entity, target, velocity, time_step);
	}

	Movable::Movable(Entity& entity)
		: m_entity(entity)
        , m_previous_position(m_entity.m_position)
	{
		entity.m_world.addTask(this, short(Task::Entity));
	}

	Movable::~Movable()
	{
		m_entity.m_world.removeTask(this, short(Task::Entity));
	}

	void Movable::setMovement(const vec3& position, const quat& rotation, const vec3& relLinVel, const vec3& relAngVel)
	{
		m_entity.setPosition(position);
		m_entity.setRotation(rotation);
		setLinearVelocity(relLinVel);
		setAngularVelocity(relAngVel);
	}

	void Movable::setAcceleration(const vec3& acceleration, const vec3& maxLinVel)
	{
		m_acceleration = acceleration;
		m_maxLinearVelocity = maxLinVel;
		m_accelerationUpdated = m_lastTick+1;
	}

	void Movable::modifyAcceleration(const vec3& acceleration)
	{
		m_acceleration += acceleration;
		m_accelerationUpdated = m_lastTick+1;
	}

    void Movable::setLinearVelocity(const vec3& linVel)
	{
		m_linear_velocity = linVel;
        m_movementUpdated = m_lastTick+1;
	}

	void Movable::modifyLinearVelocity(const vec3& linVel)
	{
		m_linear_velocity += linVel;
		m_movementUpdated = m_lastTick+1;
	}

    void Movable::setAngularVelocity(const vec3& angVel)
	{
		m_angular_velocity = angVel;
        m_movementUpdated = m_lastTick+1;
	}

	void Movable::modifyAngularVelocity(const vec3& angVel)
	{
		m_angular_velocity += angVel;
		m_movementUpdated = m_lastTick+1;
	}

    void Movable::next_frame(size_t tick, size_t delta)
	{
		if(m_acceleration != Zero3)
		{
			//m_linear_velocity += m_acceleration * (delta * float(TICK_INTERVAL));
			//if(m_linear_velocity > m_maxLinearVelocity)
			//	m_linear_velocity = m_maxLinearVelocity;
		}

		if(m_linear_velocity != Zero3 || m_angular_velocity != Zero3)
        {
			m_previous_position = m_entity.m_position;

			btVector3 linear_vel = to_btvec3(rotate(m_entity.m_rotation, m_linear_velocity));
			btVector3 angular_vel = to_btvec3(rotate(m_entity.m_rotation, m_angular_velocity));

			btTransform current(to_btquat(m_entity.m_rotation), to_btvec3(m_entity.m_position));
            btTransform updated;
            btTransformUtil::integrateTransform(current, linear_vel, angular_vel, float(delta * TICK_INTERVAL), updated);

			if(!linear_vel.isZero())
				m_entity.setPosition(to_vec3(updated.getOrigin()));
			if(!angular_vel.isZero())
				m_entity.setRotation(to_quat(updated.getRotation()));

#if 0
			printf("move entity from (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)\n", m_previous_position.x, m_previous_position.y, m_previous_position.z,
																				  m_entity.m_position.x, m_entity.m_position.y, m_entity.m_position.z);
#endif

			m_moving = true;
        }
		else
		{
			m_moving = false;
		}

		m_lastTick = tick;
    }
}
