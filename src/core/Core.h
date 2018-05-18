//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <core/Generated/Forward.h>
#include <core/World/Section.h>

#include <core/World/World.h>
#include <core/Bullet/BulletWorld.h>
#include <core/Navmesh/Navmesh.h>

/* std */
#include <map>

using namespace mud; namespace toy
{
	using string = std::string;

	class _refl_ TOY_CORE_EXPORT Core : public NonCopy
	{
	public:
		Core();
		~Core();

		void next_frame();

		TaskSection& section(short int index) { return *m_sections[index]; }

	private:
		std::map<short int, object_ptr<TaskSection>> m_sections;
	};

	class _refl_ TOY_CORE_EXPORT DefaultWorld : public Construct
	{
	public:
		_constr_ DefaultWorld(const string& name);
		~DefaultWorld();

		_comp_ _attr_ World m_world;
		_comp_ BulletWorld m_bulletWorld;
		_comp_ Navmesh m_navmesh;
	};
}
