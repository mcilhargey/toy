//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Action.h>

#include <core/Entity/Entity.h>

#include <obj/Complex.h>
#include <obj/Reflect/Method.h>

using namespace mud; namespace toy
{
	Action::Action(ProcedureType& actionType, User* user, Entity& agent, Entity& target)
		: Procedure(actionType, user, &agent, std::vector<Ref>{ &target })
		, m_agent(agent)
		, m_target(target)
	{}
}
