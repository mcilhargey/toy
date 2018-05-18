//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/World/World.h>

#include <core/World/Origin.h>

#include <obj/Indexer.h>
#include <obj/Util/Timer.h>
#include <obj/Proto.h>
#include <math/Vec.h>
#include <core/Entity/Entity.h>
#include <core/World/WorldClock.h>
#include <core/World/Section.h>

using namespace mud; namespace toy
{
	World::World(Id id, Prototype& prototype, const string& name)
        : Complex(id, type<World>(), prototype)
		, m_name(name)
		, m_clock(make_object<WorldClock>(*this))
    {
		m_sections[size_t(Task::World)] = make_object<MonoSection>(short(Task::World));
		m_sections[size_t(Task::State)] = make_object<MonoSection>(short(Task::State));
		m_sections[size_t(Task::Entity)] = make_object<MonoSection>(short(Task::Entity));
		m_sections[size_t(Task::Physics)] = make_object<MonoSection>(short(Task::Physics));
		m_sections[size_t(Task::GameObject)] = make_object<MonoSection>(short(Task::GameObject));
		m_sections[size_t(Task::Graphics)] = make_object<MonoSection>(short(Task::Graphics));

		//Index::me().indexer(type<Entity>()).alloc();
		//Index::me().indexer(type<Entity>()).alloc();
		m_origin = make_object<Origin>(1, *this);
		m_unworld = make_object<Origin>(2, *this);
	}

    World::~World()
    {}

	void World::destroy()
	{
		m_origin = nullptr;
		m_unworld = nullptr;
	}

    void World::next_frame(size_t tick, size_t delta)
    {
		UNUSED(tick); UNUSED(delta);
#ifndef TOY_THREADED
		m_clock->stepClock();
		for(auto& kv : m_sections)
			kv.second->update();
#endif
    }

    void World::addTask(Updatable* task, short int section)
    {
		m_sections[section]->addTask(task);
    }

	void World::removeTask(Updatable* task, short int section)
	{
		m_sections[section]->removeTask(task);
	}

	void World::updateTasks(short int section)
	{
		m_sections[section]->update();
	}

	TaskSection* World::section(short int section)
	{
		return m_sections[section].get();
	}
}
