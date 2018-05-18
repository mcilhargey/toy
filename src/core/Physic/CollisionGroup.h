//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

using namespace mud; namespace toy
{
	enum CollisionGroup : short int
	{
		CM_NOMASK   = 0,
		CM_OBJECT   = 1 << 0,
		CM_SOLID    = 1 << 1,
		CM_GROUND   = 1 << 2,
		CM_AREA     = 1 << 3,
		CM_BUFFER   = 1 << 4,
		CM_LIGHT    = 1 << 5,
		CM_LIGHTREFLECTOR = 1 << 6,
		CM_SOURCE   = 1 << 7,
		CM_RECEPTOR = 1 << 8,
		CM_OBSTACLE = 1 << 9
	};
}
