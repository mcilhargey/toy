//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/World/Origin.h>


using namespace mud; namespace toy
{
	Origin::Origin(Id id, World& world)
		: Construct(m_entity, proto<Origin>())
		, m_entity(id, proto<Origin>(), world, nullptr, Zero3, ZeroQuat)
	{}
}
