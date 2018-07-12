//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/NonCopy.h>
#include <math/Timer.h>
#include <visu/Forward.h>
#include <visu/VisuScene.h>

#ifdef TOY_SOUND
#include <snd/SoundManager.h>
#endif

using namespace mud; namespace toy
{
    class refl_ TOY_VISU_EXPORT VisuSystem : public NonCopy
    {
    public:
		VisuSystem(array<cstring> resource_paths);
        ~VisuSystem();

		object_ptr<GfxSystem> m_gfx_system;
#ifdef TOY_SOUND
		object_ptr<SoundManager> m_sound_system;
#endif

		void init();
		bool next_frame();

		void loadModule(VisuModule& module);
		void unloadModule(VisuModule& module);

		std::vector<VisuModule*> m_modules;
    };
}
