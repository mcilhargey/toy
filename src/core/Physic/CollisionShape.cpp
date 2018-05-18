//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Physic/CollisionShape.h>

#include <geom/Shape.h>

using namespace mud; namespace toy
{
	CollisionShape::CollisionShape(const Shape& shape, const vec3& center, float margin)
		: m_shape(shape.clone())
		, m_center(center)
		, m_margin(margin)
	{}

	CollisionShape::~CollisionShape()
	{}

	CollisionShape::CollisionShape(const CollisionShape& other)
		: CollisionShape(*other.m_shape, other.m_center, other.m_margin)
	{}

	CollisionShape& CollisionShape::operator=(const CollisionShape& other)
	{
		m_shape = other.m_shape->clone();
		m_center = other.m_center;
		m_margin = other.m_margin;
		return *this;
	}
}
