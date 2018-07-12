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
		world.add_task(this, short(Task::PhysicsWorld));
	}

    PhysicWorld::~PhysicWorld()
	{
		m_world.remove_task(this, short(Task::PhysicsWorld));
	}

	void PhysicWorld::next_frame(size_t tick, size_t delta)
	{
		for(auto& kv : m_subworlds)
			kv.second->next_frame(tick, delta);
	}

	SubPhysicWorld& PhysicWorld::sub_world(Medium& medium)
	{
		if(m_subworlds.find(&medium) == m_subworlds.end())
			m_subworlds[&medium] = this->create_sub_world(medium);
		return *m_subworlds[&medium].get();
	}
}
