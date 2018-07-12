//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <refl/Module.h>
#include <visu/Types.h>

using namespace mud; namespace toy
{
    class refl_ TOY_VISU_EXPORT VisuModule : public Module
    {
    public:
		VisuModule(cstring name, Type& type = mud::type<VisuModule>()) : Module(name) { UNUSED(type); }

		virtual void startScene(VisuScene& scene) { UNUSED(scene); }

		virtual void loadVisuModule(VisuSystem& visuSystem) { UNUSED(visuSystem); }
		virtual void unloadVisuModule(VisuSystem& visuSystem) { UNUSED(visuSystem); }
    };
}
