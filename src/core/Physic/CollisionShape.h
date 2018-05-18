//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <obj/Unique.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT CollisionShape : public NonCopy
	{
	public:
		CollisionShape(const Shape& shape, const vec3& center = Zero3, float margin = 0.f);
		~CollisionShape();

		CollisionShape(const CollisionShape& shape);
		CollisionShape& operator=(const CollisionShape& shape);

		object_ptr<Shape> m_shape;
		vec3 m_center;
		float m_margin;

		bool checkInside(const vec3& position) { UNUSED(position); return true; }
	};
}
