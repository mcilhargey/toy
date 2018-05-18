//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
#ifndef TOY_BEHAVIORAL_H
#define TOY_BEHAVIORAL_H

/* toy */
#include <obj/Complex.h>
#include <obj/NonCopy.h>
#include <core/Store/Array.h>
#include <core/Generated/Forward.h>
#include <obj/Util/Updatable.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT Behavior : public Updatable
	{
	public:
		enum Priority
		{
			BP_NONE,
			BP_LOW,
			BP_MEDIUM,
			BP_HIGH,
			BP_CRITICAL
		};

		Behavior(Type& type, Reactive& reactive);
		virtual ~Behavior();

		_attr_ Type& m_type;
		Reactive& m_reactive;
		bool m_control;
		bool m_asleep;
		Priority m_priority;
		int m_priorityValue;

		void take() { m_control = true; }
		void yield() { m_control = false; }
		
		virtual void dotake() {}
		virtual void doyield() {}

		virtual cstring name() { return ""; }

		_meth_ void sleep();
		_meth_ void wake();

		bool bypasses(Behavior* other) { return m_priority >= other->m_priority && m_priorityValue > other->m_priorityValue; }
	};

	class _refl_ TOY_CORE_EXPORT Reactive : public NonCopy, public Updatable
	{
	public:
		_constr_ Reactive(Entity& entity);
        ~Reactive();

		_attr_ Entity& m_entity;
		_attr_ Behavior* m_controller;

		std::vector<object_ptr<Behavior>> m_behaviors;

		void add(object_ptr<Behavior> behavior);

		void next_frame(size_t tick, size_t delta);

		void needControl();

		void overtake(Behavior* behavior);
		void take(Behavior* behavior);
		void wake(Behavior* behavior);
		void sleep(Behavior* behavior);

	private:
		void check(Behavior* behavior);

	private:
		std::vector<Behavior*> m_actives;
	};
}

#endif
