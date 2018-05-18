//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Symbolic/Symbolic.h>

#include <core/Entity/Entity.h>
#include <math/Colour.h>

using namespace mud; namespace toy
{
	Symbolic::Symbolic(Entity& entity, std::vector<Symbol> shapes)
		: m_entity(entity)
		, m_symbols(shapes)
		, m_updated(0)
	{}

	void Symbolic::clearSymbols()
	{
		m_symbols.clear();
		m_updated++;
	}

	void Symbolic::addSymbol(const Symbol& symbol)
	{
		m_symbols.emplace_back(symbol);
		m_updated++;
	}
}
