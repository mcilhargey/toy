//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Controller/CameraController.h>

#include <core/Entity/Entity.h>
#include <core/Entity/Entity.h>
#include <core/Movable/Movable.h>

using namespace mud; namespace toy
{
	CameraController::CameraController(Viewer& viewer, Camera& camera)
		: m_viewer(viewer)
		, m_camera(camera)
		, m_movable(camera.m_entity.part<Movable>())
		, m_velocity(50.f)
		, m_angularVelocity(2.f)
	{
		default_velocities();

		m_movable.setAcceleration(vec3(0, 0, 0.f), vec3(0, 0, m_velocity));
	}

	void CameraController::default_velocities()
	{
		std::map<KeyCode, vec3> velocities;

		velocities[KC_W] = velocities[KC_UP] = to_vec3(Side::Front) * m_velocity;
		velocities[KC_S] = velocities[KC_DOWN] = to_vec3(Side::Back) * m_velocity;
		velocities[KC_D] = velocities[KC_RIGHT] = to_vec3(Side::Right) * m_velocity;
		velocities[KC_A] = velocities[KC_LEFT] = to_vec3(Side::Left) * m_velocity;
		velocities[KC_R] = to_vec3(Side::Up) * m_velocity;
		velocities[KC_F] = to_vec3(Side::Down) * m_velocity;

		set_velocities(velocities);
	}

	void CameraController::set_velocities(const std::map<KeyCode, vec3>& velocities)
	{
		for(auto& key_velocity : velocities)
		{
			vec3 velocity = key_velocity.second;
			m_key_down_handlers[key_velocity.first] = [this, velocity] { m_movable.modifyLinearVelocity(velocity); };
			m_key_up_handlers[key_velocity.first] = [this, velocity] { m_movable.modifyLinearVelocity(-velocity); };
		}
	}

	void CameraController::stop()
	{
		m_movable.setLinearVelocity(Zero3);
		m_movable.setAngularVelocity(Zero3);
	}

	void CameraController::rotateLeft()
	{
		m_movable.modifyAngularVelocity(Y3 * -m_angularVelocity);
	}

	void CameraController::rotateRight()
	{
		m_movable.modifyAngularVelocity(Y3 * m_angularVelocity);
	}

	void CameraController::stopRotateLeft()
	{
		m_movable.modifyAngularVelocity(Y3 * m_angularVelocity);
	}

	void CameraController::stopRotateRight()
	{
		m_movable.modifyAngularVelocity(Y3 * -m_angularVelocity);

	}
	void CameraController::zoomIn()
	{
		m_camera.zoom(1.2f);//mCamera->zoom(3.f);
	}

	void CameraController::zoomOut()
	{
		m_camera.zoom(0.8f);//mCamera->zoom(-3.f);
	}

	void CameraController::pitchLensUp()
	{
		m_camera.pitchLens(M_PI/12);
	}

	void CameraController::pitchLensDown()
	{
		m_camera.pitchLens(-M_PI/12);
	}
}
