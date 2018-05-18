//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Generated/Forward.h>
#include <core/World/Origin.h>

/* std */
#include <map>
#include <string>

using namespace mud; namespace toy
{
	using string = std::string;

	class _refl_ TOY_CORE_EXPORT World : public Complex, public Updatable
    {
    public:
		_constr_ World(Id id, Prototype& prototype, const string& name);
        ~World();

		_attr_ _mut_ string m_name;
		_attr_ _graph_ Entity& origin() { return m_origin->m_entity; }
		_attr_ _graph_ Entity& unworld() { return m_unworld->m_entity; }

		void destroy();

		WorldClock& clock() { return *m_clock; }

		TaskSection* section(short int section);

    public:
        virtual void next_frame(size_t tick, size_t delta);

		void addTask(Updatable* task, short int section);
		void removeTask(Updatable* task, short int section);
		void updateTasks(short int section);

    private:
		std::map<short int, object_ptr<TaskSection>> m_sections;
		object_ptr<WorldClock> m_clock;
		object_ptr<Origin> m_origin;
		object_ptr<Origin> m_unworld;
    };
}
