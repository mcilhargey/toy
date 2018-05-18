//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Ref.h>
#include <obj/Indexer.h>
#include <obj/Util/Updatable.h>
#include <util/Generated/Forward.h>

#include <string>

using namespace mud; namespace toy
{
	using string = std::string;
	class User;

	class _refl_ TOY_UTIL_EXPORT ProcedureType
	{
	public:
		ProcedureType(Type& type);
		~ProcedureType();

		_attr_ Type& m_type;
		_attr_ uint32_t m_index;
		_attr_ string m_name;

		Meta& meta() { return *m_type.m_meta; }

		virtual bool checkObject(Ref object) = 0;
		virtual bool checkArgs(const std::vector<Ref>& args) = 0;
		virtual object_ptr<Procedure> instance(User* user, Ref object, std::vector<Ref> args) = 0;
	};

	class _refl_ TOY_UTIL_EXPORT Procedure : public Executable
	{
	public:
		Procedure(ProcedureType& def, User* user, Ref object, std::vector<Ref> args = {});

		ProcedureType& m_def;
		User* m_user;
		Ref m_object;
		std::vector<Ref> m_args;
	};

	template <class T_Procedure>
	class ProcedureDef : public ProcedureType//, public LazyGlobal<ProcedureDef<T_Procedure>>
	{
	public:
		ProcedureDef() : ProcedureType(type<T_Procedure>()) {}

		bool checkObject(Ref object) { return T_Procedure::checkObject(object); }
		bool checkArgs(const std::vector<Ref>& args) { return T_Procedure::checkArgs(args); }
		object_ptr<Procedure> instance(User* user, Ref object, std::vector<Ref> args) { return T_Procedure::instance(user, object, args); }
	};

	template <class T_Procedure>
	class TypedProcedure : public Procedure
	{
	public:
		TypedProcedure(User* user, Ref object, std::vector<Ref> args = {}) : Procedure(T_Procedure::def(), user, object, args) {}

		static bool checkObject(Ref object) { UNUSED(object); return true; }
		static bool checkArgs(const std::vector<Ref>& args) { UNUSED(args); return true; }
		static object_ptr<Procedure> instance(User* user, Ref object, std::vector<Ref> args) { return make_object<T_Procedure>(user, object, args); }

		static ProcedureType& def() { static ProcedureDef<T_Procedure> df; return df; }
	};
}
