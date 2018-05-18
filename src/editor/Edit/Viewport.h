//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <math/Vec.h>
#include <uio/Unode.h>
#include <editor/Generated/Forward.h>
#include <editor/Controller/CameraController.h>

using namespace mud; namespace toy
{
	using Selection = std::vector<Ref>;

	TOY_EDITOR_EXPORT Widget& scene_viewport(Widget& parent, VisuScene& scene, Camera& camera, Selection& selection);

	TOY_EDITOR_EXPORT void scene_vision(Widget& parent, VisuScene& scene, Selection& selection);
}
