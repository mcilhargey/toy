
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
#include <geom/Generated/Module.h>
#include <util/Generated/Module.h>
#include <core/Generated/Module.h>
#include <ui/Generated/Module.h>
#include <gfx/Generated/Module.h>
#include <snd/Generated/Module.h>
        
#include <visu/Generated/Forward.h>
#include <visu/Generated/Types.h>

#include <visu/VisuModule.h>
#include <visu/VisuPage.h>
#include <visu/VisuScene.h>
#include <visu/VisuSystem.h>
#include <visu/Ogre/OgreViewport.h>
#include <visu/Sound/SoundSource.h>

namespace toy
{
	class TOY_VISU_EXPORT toyvisu : public Module
	{
	private:
		toyvisu();

	public:
		static toyvisu& module() { static toyvisu instance; return instance; }
	};
}

#ifdef TOY_VISU_MODULE
extern "C"
TOY_VISU_EXPORT Module& getModule();
#endif
