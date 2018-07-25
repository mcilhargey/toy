//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/NonCopy.h>
#include <core/Forward.h>

#include <string>

using namespace mud; namespace toy
{
	using string = std::string;

	class refl_ TOY_CORE_EXPORT ComponentPool
	{

	};

	class refl_ TOY_CORE_EXPORT EntityScript : public NonCopy
	{
	public:
		EntityScript(Entity& entity);
		
		Entity& m_entity;
		Script* m_logic_script = nullptr;
		Script* m_render_script = nullptr;
	};
}
