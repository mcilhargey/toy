//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <core/Forward.h>
#include <core/World/Origin.h>

#ifndef MUD_CPP_20
#include <vector>
#include <string>
#endif

using namespace mud; namespace toy
{
	using string = std::string;

	class refl_ TOY_CORE_EXPORT World : public Updatable
    {
    public:
		constr_ World(Id id, Complex& complex, const string& name);
        ~World();

		attr_ Id m_id;
		attr_ Complex& m_complex;
		attr_ mut_ string m_name;
		attr_ graph_ Entity& origin() { return m_origin->m_entity; }
		attr_ graph_ Entity& unworld() { return m_unworld->m_entity; }

		void destroy();

		WorldClock& clock() { return *m_clock; }

		TaskSection* section(short int section);

    public:
        virtual void next_frame(size_t tick, size_t delta);

		void add_task(Updatable* task, short int section);
		void remove_task(Updatable* task, short int section);
		void updateTasks(short int section);

	public:
		template <class T>
		inline bool isa() { return m_complex.isa<T>(); }

		template <class T>
		inline T& part() { return m_complex.part<T>(); }

    private:
		std::vector<object_ptr<TaskSection>> m_sections;
		object_ptr<WorldClock> m_clock;
		object_ptr<Origin> m_origin;
		object_ptr<Origin> m_unworld;
    };
}
