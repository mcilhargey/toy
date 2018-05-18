//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Store/Array.h>
#include <core/Generated/Types.h>
#include <core/Entity/EntityObserver.h>
#include <core/Selector/Selection.h>

using namespace mud; namespace toy
{
	class TargetDispatcher
	{
	public:
		//virtual void init(const ActorList& actors) = 0;
		virtual void init(const std::vector<Ref>& targets) = 0;
		virtual Ref dispatchTarget(Ref agent) = 0;
	};

	//void selector_actions()

	class _refl_ TOY_CORE_EXPORT Selector : public StoreObserver<Entity>
	{
	public:
		_constr_ Selector(User& user);
		~Selector();

		_attr_ User& m_user;

		Selection m_selection;
		Selection m_targets;

		Array<ProcedureType> m_actions;
		Array<ProcedureType> m_methods;

		void updateActions();
		void updateMethods();

		void handleAdd(Entity& entity);
		void handleRemove(Entity& entity);
		bool handleClear();

		//void execute(ProcedureType& method);
		void execute(ProcedureType& action);

		//void execute(const string& action);

		void execute(ProcedureType& action, TargetDispatcher& dispatcher);
	};
}
