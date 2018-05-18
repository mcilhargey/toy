//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <geom/Shape.h>
#include <geom/Symbol.h>
#include <core/Generated/Forward.h> 

#include <vector>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT Symbolic
	{
	public:
		_constr_ Symbolic(Entity& entity, std::vector<Symbol> shapes = {});

		_attr_ Entity& m_entity;
		_attr_ std::vector<Symbol> m_symbols;
		_attr_ size_t m_updated;

		void clearSymbols();
		void addSymbol(const Symbol& symbol);
	};
}
