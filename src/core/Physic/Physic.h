//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <infra/Global.h>
#include <core/Forward.h>
#include <core/Physic/Medium.h>
#include <core/Physic/Scope.h>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT PhysicMedium : public Medium, public LazyGlobal<PhysicMedium>
	{
	public:
		PhysicMedium();
	};

	class TOY_CORE_EXPORT Physic
	{
	public:
		Physic(Entity& entity);

	protected:
		//Array<Area> m_areaStore;
	};
}
