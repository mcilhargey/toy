//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Core.h>

#include <core/Player/Player.h>
#include <core/World/Section.h>

#define TOY_POOL_IMPLEMENTATION
#include <obj/Memory/Pool.h>
#include <obj/Memory/ObjectPool.h>

#include <obj/Reflect/Injector.h>
#include <obj/System/System.h>


using namespace mud; namespace toy
{
	Core::Core()
	{
		m_sections[size_t(Task::World)] = make_object<MonoSection>(short(Task::World));
		m_sections[size_t(Task::Entity)] = make_object<MonoSection>(short(Task::Entity));
		m_sections[size_t(Task::State)] = make_object<MonoSection>(short(Task::State));
		m_sections[size_t(Task::Physics)] = make_object<MonoSection>(short(Task::Physics));
		m_sections[size_t(Task::GameObject)] = make_object<MonoSection>(short(Task::GameObject));
		m_sections[size_t(Task::Ui)] = make_object<MonoSection>(short(Task::Ui));
		m_sections[size_t(Task::Graphics)] = make_object<MonoSection>(short(Task::Graphics));
		m_sections[size_t(Task::Core)] = make_object<MonoSection>(short(Task::Core));
		m_sections[size_t(Task::Cleanup)] = make_object<MonoSection>(short(Task::Cleanup));

		//m_sections[TASK_LOADER] = make_object<QueueSection>(TASK_CLEAN);
	}

	Core::~Core()
	{}

	void Core::next_frame()
	{
		for(auto& kv : m_sections)
			kv.second->update();
	}

	DefaultWorld::DefaultWorld(const string& name)
		: Construct(m_world, proto<DefaultWorld>())
		, m_world(0, proto<DefaultWorld>(), name)
		, m_bulletWorld(m_world)
		, m_navmesh(m_world)
	{
		// @5603 : adding to nested only when object is finish -> in prototype
		this->index(m_bulletWorld, m_navmesh, *this);
	}

	DefaultWorld::~DefaultWorld()
	{
		m_world.destroy();
	}
}
