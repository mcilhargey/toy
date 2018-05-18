//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>
#include <core/Store/StoreObserver.h>

using namespace mud; namespace toy
{
	template class TOY_CORE_EXPORT StoreObserver<Entity>;

	class HookObserver { public: virtual void hooked() = 0;
								 virtual void unhooked() = 0; };
}
