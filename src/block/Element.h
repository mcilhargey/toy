//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <obj/Proto.h>
#include <obj/Util/Global.h>
#include <math/Colour.h>
#include <math/Grid.h>
#include <math/Math.h>
#include <core/Entity/Entity.h>
#include <core/Physic/Scope.h>
#include <block/Generated/Forward.h>

/* std */
#include <vector>
#include <string>

using namespace mud; namespace toy
{
	using string = std::string;

	enum _refl_ MatterState : unsigned int
	{
		STATE_SOLID,
		STATE_LIQUID,
		STATE_GAS,
		STATE_PLASMA
	};

	class _refl_ TOY_BLOCK_EXPORT Element
	{
	public:
		_constr_ Element(cstring name, MatterState state, Colour colour);

		_attr_ Id m_id;
		_attr_ string m_name;
		_attr_ MatterState m_matterState;
		_attr_ Colour m_colour;
	};

	class _refl_ TOY_BLOCK_EXPORT Heap : public Construct
	{
	public:
		_constr_ Heap(Id id, Entity& parent, const vec3& position, Element& element, float radius);

		_comp_ _attr_ Entity m_entity;
		_attr_ _link_  Element& m_element;
		_attr_ float m_radius;

		//_attr_ Array<Chunk>& chunks() { return m_chunks; }

	protected:
		//Array<Chunk> m_chunks;
	};

	class TOY_BLOCK_EXPORT ChunkFilter
	{
	public:
		virtual bool filter(Chunk& chunk) = 0;
	};

	class TOY_BLOCK_EXPORT GroundChunk : public LazyGlobal<GroundChunk>, public ChunkFilter
	{
	public:
		bool filter(Chunk& cell);
	};
}
