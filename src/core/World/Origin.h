//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Proto.h>
#include <core/Forward.h>
#include <core/Entity/Entity.h>

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Origin : public Complex
	{
	public:
		constr_ Origin(Id id, World& world);

		comp_ attr_ Entity m_entity;
	};
}
