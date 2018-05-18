//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
/* toy */

#include <core/World/Section.h>

#include <obj/Util/Updatable.h>
#include <obj/Vector.h>

#include <memory>
#include <thread>

#define NUM_THREADS 2

using namespace mud; namespace toy
{
	TaskSection::TaskSection(short int id)
		: m_id(id)
		, m_clock()
		, m_lastTick(0)
	{}

	TaskSection::~TaskSection()
	{}

	QueueSection::QueueSection(short int id)
		: TaskSection(id)
		, m_thread(make_unique<std::thread>([this] { this->update(); }))
		, m_taskQueue(50)
	{}

	void QueueSection::update()
	{
		if(!m_taskQueue.empty())
		{
			TaskFunc task;
			m_taskQueue.pop(task);
			task();
		}
	}

	MonoSection::MonoSection(short int id)
		: TaskSection(id)
		, m_tasks()
#ifdef TOY_THREADED
		, m_thread(make_unique<std::thread([this] { this->update(); }))
#endif
	{}

	void MonoSection::addTask(Updatable* task)
	{
		m_tasks.push_back(task);
	}

	void MonoSection::removeTask(Updatable* task)
	{
		vector_remove(m_tasks, task);
	}

	void MonoSection::update()
	{
		size_t tick = m_clock.readTick();
		size_t delta = m_clock.stepTick();

		m_lastTick = tick;

		size_t i = 0;

		m_tasksBuffer.clear();
		m_tasksBuffer.assign(m_tasks.begin(), m_tasks.end());

		for(Updatable* task : m_tasksBuffer)
		{
			task->next_frame(tick, delta);
			++i;
		}
	}

	ParallelSection::ParallelSection(short int id)
		: TaskSection(id)
		, m_thread(make_unique<std::thread>([this] { this->update(); }))
		, m_workers()
		, m_taskCursor(0)
		, m_taskQueueSize(50)
		, m_taskQueue(50)
	{
		for(int i = 0; i != NUM_THREADS; ++i)
			m_workers.emplace_back(make_unique<std::thread>([this] { this->workerUpdate(); }));
	}

	void ParallelSection::update()
	{
		if(!m_taskQueue.empty())
			return;

		//int numTasks = 0;
		//for(; numTasks != m_taskQueueSize && m_taskCursor != m_world->getTasks(m_id).size(); ++numTasks, ++mTaskCursor)
		//	m_taskQueue.push(m_world->getTasks(m_id)[mTaskCursor]);
		
		m_taskCursor += m_taskQueueSize;
	}

	void ParallelSection::workerUpdate()
	{
		if(m_taskQueue.empty())
			return;

		// Symbolic time : double timeStep = m_world->stepClock(m_clock->step());
		size_t tick = m_clock.readTick();
		size_t delta = m_clock.stepTick();

		m_lastTick = tick;

		Updatable* task;
		m_taskQueue.pop(task);
		task->next_frame(tick, delta);
	}

}
