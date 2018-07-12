//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <edit/Controller/FPSCameraController.h>

#include <core/Entity/Entity.h>

#include <ui/Frame/Frame.h>
#include <ui/Widget.h>
#include <ui/Cursor.h>
#include <ui/Structs/RootSheet.h>
#include <ctx/InputDevice.h>

#include <gfx-ui/Viewer.h>

using namespace mud; namespace toy
{
	FPSCameraController::FPSCameraController(Viewer& viewer, Camera& camera)
		: CameraController(viewer, camera)
	{
		m_key_down_handlers[KC_Q] = [this] { this->rotateLeft(); };
		m_key_down_handlers[KC_E] = [this] { this->rotateRight(); }; 
	}

	void FPSCameraController::process(Viewer& viewer)
	{
		EventDispatch::process(viewer);

		//if(active)
		//m_inputWidget->root_sheet().m_cursor.hide();
		//else
		//m_inputWidget->root_sheet().m_cursor.show();

		if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::Mouse, EventType::Moved))
		{
			vec2 angle = mouse_event.m_delta / viewer.m_frame.m_size;
			m_camera.m_entity.pitch(-angle.x * 4);
			m_camera.m_entity.yaw_fixed(-angle.y * 4);
		}
	}
}
