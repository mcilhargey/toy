//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <obj/Ref.h>
#include <obj/Indexer.h>
#include <core/Forward.h>

#ifndef MUD_CPP_20
#include <vector>
#endif

using namespace mud; namespace toy
{
	using string = std::string;

	class refl_ TOY_CORE_EXPORT User
	{
	public:
		std::vector<Ref> m_selection;
	};

#if 0
	class refl_ TOY_CORE_EXPORT Player
	{
	public:
		constr_ Player(Id id, const string& name);

		attr_ mut_ Id m_id;
		attr_ mut_ string m_name;
		attr_ mut_ std::vector<Ref> m_selection;
	};
#endif
}
