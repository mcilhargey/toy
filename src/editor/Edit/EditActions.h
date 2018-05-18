//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <util/Procedure.h>
#include <editor/Generated/Types.h>


using namespace mud; namespace toy
{
	class _refl_ TOY_EDITOR_EXPORT Edit : public TypedProcedure<Edit>
	{
	public:
		Edit(User* user, Ref object, std::vector<Ref> args) : TypedProcedure(user, object) { UNUSED(args); }

		static bool checkObject(Ref object) { UNUSED(object); return true; }

		void begin();
	};

	class _refl_ TOY_EDITOR_EXPORT Clone : public TypedProcedure<Clone>
	{
	public:
		Clone(User* user, Ref object, std::vector<Ref> args) : TypedProcedure(user, object) { UNUSED(args); }

		static bool checkObject(Ref object) { UNUSED(object); return true; }

		void begin();
	};

	class _refl_ TOY_EDITOR_EXPORT Cut : public TypedProcedure<Cut>
	{
	public:
		Cut(User* user, Ref object, std::vector<Ref> args) : TypedProcedure(user, object) { UNUSED(args); }

		void begin() {}
	};

	class _refl_ TOY_EDITOR_EXPORT Paste : public TypedProcedure<Paste>
	{
	public:
		Paste(User* user, Ref object, std::vector<Ref> args) : TypedProcedure(user, object) { UNUSED(args); }

		void begin() {}
	};
}
