//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Ref.h>
#include <obj/Indexer.h>
#include <core/Generated/Forward.h>

/* std */
#include <vector>

using namespace mud; namespace toy
{
	using string = std::string;

	class _refl_ TOY_CORE_EXPORT User
	{
	public:
		std::vector<Ref> m_selection;
	};

	class _refl_ TOY_CORE_EXPORT Player
	{
	public:
		_constr_ Player(Id id, const string& name);

		_attr_ _mut_ Id m_id;
		_attr_ _mut_ string m_name;
		_attr_ _mut_ std::vector<Ref> m_selection;
	};
}
