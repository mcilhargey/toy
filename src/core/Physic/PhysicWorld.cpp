//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Physic/PhysicWorld.h>

#include <core/World/Section.h>
#include <core/World/World.h>

using namespace mud; namespace toy
{
	SubPhysicWorld::SubPhysicWorld(World& world, Medium& medium)
		: m_world(world)
		, m_medium(medium)
	{}

    PhysicWorld::PhysicWorld(World& world)
		: m_world(world)
	{
		world.addTask(this, short(Task::Physics));
	}

    PhysicWorld::~PhysicWorld()
	{
		m_world.removeTask(this, short(Task::Physics));
	}

	void PhysicWorld::next_frame(size_t tick, size_t delta)
	{
		for(auto& kv : m_subWorlds)
			kv.second->next_frame(tick, delta);
	}

	SubPhysicWorld& PhysicWorld::getSubWorld(Medium& medium)
	{
		if(m_subWorlds.find(&medium) == m_subWorlds.end())
			m_subWorlds[&medium] = this->createSubWorld(medium);
		return *m_subWorlds[&medium].get();
	}
}
