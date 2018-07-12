//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Camera/Camera.h>

#include <refl/Class.h>
#include <math/Anim/Anim.h>
#include <math/Random.h>

#include <core/Entity/Entity.h>
#include <core/World/World.h>
#include <core/WorldPage/WorldPage.h>

#include <core/World/Section.h>

using namespace mud; namespace toy
{
    Camera::Camera(Entity& entity, float lensDistance, float nearClipDistance, float farClipDistance)
        : m_entity(entity)
		, m_lensDistance(lensDistance)
		, m_nearClipDistance(nearClipDistance)
        , m_farClipDistance(farClipDistance)
    {
		entity.m_world.add_task(this, short(Task::Entity));
		calcLensPosition();
	}

	Camera::~Camera()
	{
		m_entity.m_world.remove_task(this, short(Task::Entity));
	}
	
	void Camera::setLensDistance(float distance)
	{
		m_lensDistance = distance;
		m_lensUpdated = true;
	}

	void Camera::zoom(float amount)
	{
		m_lensDistance *= 1.f/amount;
		m_lensUpdated = true;
	}

	void Camera::setLensAngle(float angle)
	{
		m_lensAngle = angle;
		m_lensUpdated = true;
	}

	void Camera::pitchLens(float amount)
	{
		m_lensAngle += amount;
		m_lensUpdated = true;
	}

	vec3 Camera::lensDirection()
	{
		return rotate(m_entity.front(), -m_lensAngle, m_entity.right());
	}

	void Camera::calcLensPosition()
	{
		m_lensPosition = -this->lensDirection() * m_lensDistance;
		m_lensPosition += m_entity.absolute_position();
	}

	void Camera::calcLensRotation()
	{
		m_lensRotation = m_entity.absolute_rotation() * angleAxis(-m_lensAngle, to_vec3(Side::Right));
	}

	void Camera::next_frame(size_t tick, size_t delta)
	{
		UNUSED(delta);

		if(m_entity.m_last_updated > m_last_updated || m_lensUpdated)
		{
			calcLensPosition();
			calcLensRotation();

			for(CameraObserver* observer : m_observers)
				observer->cameraUpdated(m_lensPosition, m_lensRotation, m_entity.absolute_position(), m_nearClipDistance, m_farClipDistance);
		}
		m_last_updated = tick;
		m_lensUpdated = false;
	}

	void Camera::observe(CameraObserver& observer)
	{
		m_observers.push_back(&observer);
		observer.cameraUpdated(m_lensPosition, m_lensRotation, m_entity.m_position, m_nearClipDistance, m_farClipDistance);
	}

	void Camera::unobserve(CameraObserver& observer)
	{
		vector_remove(m_observers, &observer);
	}

	OCamera::OCamera(Id id, Entity& parent, const vec3& position, float lensDistance, float nearClipDistance, float farClipDistance)
		: Complex(id, type<OCamera>(), m_movable, m_receptor, m_camera)
		, m_entity(id, *this, parent, position, ZeroQuat)
		, m_movable(m_entity)
		, m_receptor(m_entity)
		, m_camera(m_entity, lensDistance, nearClipDistance, farClipDistance)
	{
		m_receptor.addSphere(WorldMedium::me(), 0.1f, CM_RECEPTOR);

		// @5603 : adding to nested only when object is finish -> in prototype
		m_entity.m_parent->m_contents.add(m_entity);
	}

	void jump_camera_to(Camera& camera, const vec3& target)
	{
		animate(Ref(&camera), member(&Camera::m_lensDistance), var(random_scalar(1.f, 2.f)), 1.f);
		animate(Ref(&camera.m_entity), member(&Entity::m_position), var(target), 1.f);
		animate(Ref(&camera.m_entity), member(&Entity::m_rotation), var(rotate(camera.m_entity.m_rotation, random_scalar(float(-c_pi / 8.f), float(c_pi / 8.f)), Y3)), 1.f);
	}

	void move_camera_to(Camera& camera, const vec3& target)
	{
		animate(Ref(&camera.m_entity), member(&Entity::m_position), var(target), 1.f);
	}
}
