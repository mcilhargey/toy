//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is licensed  under the terms of the GNU General Public License v3.0.
//  See the attached LICENSE.txt file or https://www.gnu.org/licenses/gpl-3.0.en.html.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <edit/Controller/CameraController.h>

using namespace mud; namespace toy
{
	class TOY_EDIT_EXPORT FPSCameraController : public CameraController
	{
	public:
		FPSCameraController(Viewer& viewer, Camera& camera);

		virtual void process(Widget& widget) { UNUSED(widget); }
		virtual void process(Viewer& viewer);
	};
}
