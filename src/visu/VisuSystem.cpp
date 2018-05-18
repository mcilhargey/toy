//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <visu/VisuSystem.h>

#include <core/View/Vision.h>

#include <gfx/GfxSystem.h>

#include <visu/VisuScene.h>

#include <visu/VisuModule.h>

namespace mud
{
#ifdef TOY_SOUND
	template <> Type& type<SoundManager>() { static Type ty("SoundManager"); return ty; }
#endif
}

using namespace mud; namespace toy
{

	VisuSystem::VisuSystem(array<cstring> resource_paths)
		: m_gfx_system(make_object<GfxSystem>(resource_paths))
#ifdef TOY_SOUND
		, m_sound_system(make_object<SoundManager>(resource_paths[0]))
#endif
	{}

	VisuSystem::~VisuSystem()
	{}

	void VisuSystem::init()
	{
#ifdef TOY_SOUND
		m_sound_system->init();
#endif
	}

	bool VisuSystem::next_frame()
	{
		return m_gfx_system->next_frame();
	}

	void VisuSystem::loadModule(VisuModule& module)
	{
		module.loadVisuModule(*this);
		m_modules.push_back(&module);
	}

	void VisuSystem::unloadModule(VisuModule& module)
	{
		module.unloadVisuModule(*this);
		vector_remove(m_modules, &module);
	}
}
