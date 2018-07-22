//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <core/Camera/Camera.h>
#include <ui/Controller/Controller.h>
#include <gfx-ui/Viewer.h>
#include <edit/Forward.h>

using namespace mud; namespace toy
{
	class TOY_EDIT_EXPORT CameraController : public ViewerController, public EventDispatch
	{
	public:
		CameraController(Viewer& viewer, Camera& camera);

		void default_velocities();
		void set_velocities(const std::map<KeyCode, vec3>& velocities);

		void rotate_left();
		void rotate_right();

		void stop_rotate_left();
		void stop_rotate_right();

		void stop();

		void zoom_in();
		void zoom_out();

		void pitch_lens_up();
		void pitch_lens_down();

	protected:
		Viewer& m_viewer;
		Camera& m_camera;
		Movable& m_movable;
		float m_velocity;
		float m_angular_velocity;
	};
}
