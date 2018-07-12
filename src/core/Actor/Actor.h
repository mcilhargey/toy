//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <infra/Updatable.h>
#include <core/Forward.h>
#include <core/Store/Array.h>
#include <core/Action.h>

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Actor : public NonCopy, public Updatable
	{
	public:
		constr_ Actor(Entity& entity);
        ~Actor();

		 attr_ Entity& m_entity;

		 Array<ProcedureType> m_actions;
		 std::vector<object_ptr<Procedure>> m_queued;
		 std::vector<object_ptr<Procedure>> m_execution;

		void next_frame(size_t tick, size_t delta);

		void halt();

		Procedure& execute(object_ptr<Procedure> action);
		Procedure& queue(object_ptr<Procedure> action);

		Procedure& queue(User* user, ProcedureType& action, Entity& target);
		Procedure& execute(User* user, ProcedureType& action, Entity& target);

		//Procedure* queue(User* user, const string& action, Entity* target);
		//Procedure* execute(User* user, const string& action, Entity* target);

		//void queueBatch(Procedure* action, std::vector<Ref> targets);

	private:
	};
}
