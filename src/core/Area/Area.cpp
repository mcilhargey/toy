//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Area/Area.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Collider.h>
#include <core/Physic/CollisionShape.h>
#include <core/Physic/Physic.h>

using namespace mud; namespace toy
{
	Area::Area(Entity& entity, const CollisionShape& shape)
		: m_collider(entity, *this, shape, PhysicMedium::me(), CM_AREA)
	{}

	Area::~Area()
	{}

	bool Area::checkInside(const vec3& position)
	{
		return m_collider.m_collision_shape.checkInside(position);
	}
}
