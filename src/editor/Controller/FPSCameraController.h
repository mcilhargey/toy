//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <editor/Controller/CameraController.h>

using namespace mud; namespace toy
{
	class TOY_EDITOR_EXPORT FPSCameraController : public CameraController
	{
	public:
		FPSCameraController(Viewer& viewer, Camera& camera);

		virtual void process(Viewer& viewer);
	};
}
