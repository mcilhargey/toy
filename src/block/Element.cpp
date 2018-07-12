//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <block/Types.h>
#include <block/Element.h>

#include <core/Entity/Entity.h>
#include <core/WorldPage/WorldPage.h>

#include <block/Sector.h>
#include <block/Chunk.h>

using namespace mud; namespace toy
{
	bool GroundChunk::filter(Chunk& chunk)
	{
		return (chunk.m_element.m_state == MatterState::Gas
			 && chunk.neighbour(Side::Down)->m_element.m_state == MatterState::Solid);
	}

	Element::Element(cstring name, MatterState state, Colour colour)
		: m_name(name)
		, m_state(state)
		, m_colour(colour)
	{}

	Heap::Heap(Id id, Entity& parent, const vec3& position, Element& element, float radius)
		: Complex(id, type<Heap>(), *this)
		, m_entity(id, *this, parent, position, ZeroQuat)
		, m_element(element)
		, m_radius(radius)
	{
		// @5603 : adding to nested only when object is finish -> in prototype
		m_entity.m_parent->m_contents.add(m_entity);
	}
}
