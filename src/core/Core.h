//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/NonCopy.h>
#include <core/Forward.h>
#include <core/World/Section.h>

#include <core/World/World.h>
#include <core/Bullet/BulletWorld.h>
#include <core/Navmesh/Navmesh.h>

#ifndef MUD_CPP_20
#include <map>
#endif

using namespace mud; namespace toy
{
	using string = std::string;

	class refl_ TOY_CORE_EXPORT Core : public NonCopy
	{
	public:
		Core();
		~Core();

		void next_frame();

		TaskSection& section(short int index) { return *m_sections[index]; }

	private:
		std::vector<object_ptr<TaskSection>> m_sections;
	};

	class refl_ TOY_CORE_EXPORT DefaultWorld : public Complex
	{
	public:
		constr_ DefaultWorld(const string& name);
		~DefaultWorld();

		comp_ attr_ World m_world;
		comp_ attr_ BulletWorld m_bullet_world;
		comp_ attr_ Navmesh m_navmesh;
	};
}
