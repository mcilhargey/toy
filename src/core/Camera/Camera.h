//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Proto.h>
#include <math/Math.h>
#include <math/Vec.h>
#include <infra/Updatable.h>
#include <core/Forward.h>
#include <core/Entity/Entity.h>
#include <core/Movable/Movable.h>
#include <core/Physic/Scope.h>
#include <core/Entity/EntityObserver.h>
#include <core/Camera/CameraObserver.h>

#ifndef MUD_CPP_20
#include <functional>
#include <array>
#endif

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Camera : public Updatable
	{
	public:
		constr_ Camera(Entity& entity, float lensDistance = 1.f, float nearClipDistance = 0.001f, float farClipDistance = 1000.f);
		~Camera();

		attr_ Entity& m_entity;

		attr_ mut_ float m_lensDistance;
		attr_ mut_ float m_lensAngle = 0.f;

		attr_ mut_ float m_nearClipDistance;
		attr_ mut_ float m_farClipDistance;

		attr_ mut_ float m_aspectRatio;

		bool m_lensUpdated = true;

		vec3 m_lensPosition = Zero3;
		quat m_lensRotation = ZeroQuat;

		size_t m_last_updated = 0;

		void next_frame(size_t tick, size_t delta);

		void calcLensPosition();
		void calcLensRotation();

		void setLensDistance(float distance);
		void zoom(float amount);

		void setLensAngle(float angle);
		void pitchLens(float amount);

		//void viewportRay(float tx, float ty, vec3& from, vec3& to);
		//void planarRay(float tx, float ty, vec3& from, vec3& to);

		vec3 lensDirection();

		void observe(CameraObserver& observer);
		void unobserve(CameraObserver& observer);

	private:
		bool m_planar;
		float m_verticalAngle;
		float m_planeDistance;

#ifdef TOY_THREADED
		TransfomReceiver m_receiver;
#endif

		std::vector<CameraObserver*> m_observers;
	};

	class refl_ TOY_CORE_EXPORT OCamera : public Complex
	{
	public:
		constr_ OCamera(Id id, Entity& parent, const vec3& position, float lensDistance, float nearClipDistance = 0.001f, float farClipDistance = 1000.f);
		
		comp_ attr_ Entity m_entity;
		comp_ attr_ Movable m_movable;
		comp_ attr_ Receptor m_receptor;
		comp_ attr_ Camera m_camera;
	};

	export_ TOY_CORE_EXPORT void jump_camera_to(Camera& camera, const vec3& position);
	export_ TOY_CORE_EXPORT void move_camera_to(Camera& camera, const vec3& position);
}
