//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <geom/Shape.h>
#include <geom/Symbol.h>
#include <core/Forward.h> 

#ifndef MUD_CPP_20
#include <vector>
#endif

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Symbolic
	{
	public:
		constr_ Symbolic(Entity& entity, std::vector<Symbol> shapes = {});

		attr_ Entity& m_entity;
		attr_ std::vector<Symbol> m_symbols;
		attr_ size_t m_updated;

		void clearSymbols();
		void addSymbol(const Symbol& symbol);
	};
}
