//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Medium/SoundMedium.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Collider.h>

using namespace mud; namespace toy
{
	SoundMedium::SoundMedium()
		: Medium("SoundMedium")
	{
		m_masks[CM_OBSTACLE] = CM_NOMASK;
		m_masks[CM_SOURCE] = CM_RECEPTOR;
		m_masks[CM_RECEPTOR] = CM_SOURCE;
	}
}
