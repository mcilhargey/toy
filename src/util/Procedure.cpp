//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <util/Types.h>
#include <util/Procedure.h>

#include <proto/Complex.h>
#include <refl/Method.h>

using namespace mud; namespace toy
{
	ProcedureType::ProcedureType(Type& type)
		: m_type(type), m_index(index(type, Ref(this)))
	{}

	ProcedureType::~ProcedureType()
	{
		unindex(m_type, m_index);
	}

	Procedure::Procedure(ProcedureType& def, User* user, Ref object, std::vector<Ref> args)
		: m_def(def)
		, m_user(user)
		, m_object(object)
		, m_args(std::move(args))
	{}
}
