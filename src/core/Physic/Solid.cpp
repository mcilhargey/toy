//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Physic/Solid.h>

using namespace mud; namespace toy
{
	SolidMedium::SolidMedium()
		: Medium("SolidMedium")
	{
		m_solid = true;
		m_masks[CM_SOLID] = CM_SOLID | CM_GROUND;
		m_masks[CM_GROUND] = CM_GROUND | CM_SOLID;
	}
}
