//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <math/Math.h>
#include <block/Generated/Forward.h>
#include <core/Entity/Entity.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_BLOCK_EXPORT Chunk : public Construct
	{
	public:
		_constr_ Chunk(Id id, Entity& parent, const vec3& position, size_t index, Element& element, float size);

		_comp_ _attr_ Entity m_entity;

		_attr_ size_t m_index;
		_attr_ Block& m_block;
		_attr_ Element& m_element;
		_attr_ float m_size;

		Sector& sector();

		Chunk* neighbour(Side side);
		bool boundary(Side side);
	};
}
