//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Util/LocklessQueue.h>
#include <obj/Unique.h>
#include <obj/Util/Timer.h>
#include <core/Generated/Forward.h>

/* std */
#include <vector>
#include <thread>
#include <algorithm>

using namespace mud; namespace toy
{
	enum class Task : unsigned int
	{
		World = 0,
		Entity = 1,
		State = 2,
		Physics = 3,
		GameObject = 4,
		Ui = 5,
		Graphics = 6,
		Core = 7,
		Cleanup = 8,
		Loader = 9
	};

	/* Threading recap :
			TaskSection 0 - MonoThread
				BulletWorld next_frame
					read write BulletCollider (CONFLICT with section 2)

			TaskSection 1 - MonoThread
				Actor next_frame
					Observer handleSetCurrentAction
					Action	execute
							next_frame

				Active next_frame
					Effect update

				Camera next_frame
					Observer cameraUpdated

				Entity next_frame
					Observer transformUpdated

				Movable
					Manipulator update
					Observer movementUpdated
								accelerationUpdated
					Entity updatePosition
							updateRotation

				Manoeuvrable next_frame (lock : position)
					MotionStrategy update (position)

			TaskSection 2 - MonoThread
				Collider next_frame
					receive entity position
					read write BulletCollider (CONFLICT with section 0)

			TaskSection 3 - MonoThread
				Reactive next_frame
					Behavior update

				Human next_frame
	*/

	class _refl_ TOY_CORE_EXPORT TaskSection
	{
	public:
		TaskSection(short int id);
		virtual ~TaskSection();

		virtual void update() = 0;
		
		virtual void addTask(Updatable* task) = 0;
		virtual void removeTask(Updatable* task) = 0;

		short int m_id;
		Clock m_clock;
		size_t m_lastTick;
	};

	typedef std::function<void()> TaskFunc;

	class _refl_ TOY_CORE_EXPORT QueueSection : public TaskSection
	{
	public:
		QueueSection(short int id);

		void update();

		void addTask(Updatable* task) { UNUSED(task); }
		void removeTask(Updatable* task) { UNUSED(task); }

		void addTask(TaskFunc task) { m_taskQueue.push(task); }

	private:
		unique_ptr<std::thread> m_thread;
		LocklessQueue<TaskFunc> m_taskQueue;
	};

	class _refl_ TOY_CORE_EXPORT MonoSection : public TaskSection
	{
	public:
		MonoSection(short int id);

		void update();

		void addTask(Updatable* task);
		void removeTask(Updatable* task);

	private:
		unique_ptr<std::thread> m_thread;
		std::vector<Updatable*> m_tasks;
		std::vector<Updatable*> m_tasksBuffer;
		//std::vector<Updatable*> m_add;
		//std::vector<Updatable*> m_remove;
	};

	class _refl_ TOY_CORE_EXPORT ParallelSection  : public TaskSection
	{
	public:
		ParallelSection(short int id);

		void update();
		void workerUpdate();

	private:
		unique_ptr<std::thread> m_thread;
		std::vector<unique_ptr<std::thread>> m_workers;

		int m_taskCursor;
		int m_taskQueueSize;
		LocklessQueue<Updatable*> m_taskQueue;
	};
}
