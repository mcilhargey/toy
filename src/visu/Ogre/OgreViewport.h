//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <visu/Forward.h>
#include <core/Camera/CameraObserver.h>
#include <gfx-ui/Viewer.h>

using namespace mud; namespace toy
{
	class TOY_VISU_EXPORT OgreViewport : public Viewer //, public CameraObserver
	{
	public:
		OgreViewport(Widget* parent, void* identity, Scene& scene/*, VisuCamera& camera*/);
	};
}
