//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <util/Procedure.h>
#include <core/Generated/Forward.h>

#include <core/Entity/Entity.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT Action : public Procedure
	{
	public:
		Action(ProcedureType& type, User* user, Entity& agent, Entity& target);

	protected:
		Entity& m_agent;
		Entity& m_target;
	};

	template <class T_Action>
	class TypedAction : public Action
	{
	public:
		TypedAction(User* user, Entity& agent, Entity& target) : Action(T_Action::def(), user, agent, target) {}

		static bool checkObject(Ref object) { return T_Action::checkAgent(val<Entity>(object)); }
		static bool checkArgs(const std::vector<Ref>& args) { return T_Action::checkTarget(val<Entity>(args[0])); }
		static object_ptr<Procedure> instance(User* user, Ref object, std::vector<Ref> args) { return make_object<T_Action>(user, val<Entity>(object), val<Entity>(args[0])); }

		static ProcedureType& def() { static ProcedureDef<T_Action> instance; return instance; }
	};

	template class TOY_CORE_EXPORT Array<Action>;
	template class TOY_CORE_EXPORT StoreObserver<Action>;
}
