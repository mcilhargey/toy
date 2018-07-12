//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <math/Vec.h>
#include <uio/Unode.h>
#include <edit/Forward.h>
#include <edit/Controller/CameraController.h>

using namespace mud; namespace toy
{
	using Selection = std::vector<Ref>;

	TOY_EDIT_EXPORT Viewer& scene_viewport(Widget& parent, VisuScene& scene, Camera& camera, Selection& selection);
}
