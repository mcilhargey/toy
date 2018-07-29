//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Medium/VisualMedium.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Collider.h>

using namespace mud; namespace toy
{
	VisualMedium VisualMedium::me;

	VisualMedium::VisualMedium()
		: Medium("VisualMedium")
	{	
		m_masks[CM_LIGHT] = CM_LIGHTREFLECTOR;
		m_masks[CM_LIGHTREFLECTOR] = CM_LIGHT;
	}
}
