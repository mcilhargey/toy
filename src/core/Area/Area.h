//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <math/Math.h>
#include <core/Forward.h>
#include <core/Physic/Collider.h>

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Area : public ColliderObject
	{
	public:
		constr_ Area(Entity& entity, const CollisionShape& shape);
		~Area();

		attr_ inline Entity& entity() { return m_collider.m_entity; }
		attr_ inline CollisionShape& shape() { return m_collider.m_collision_shape; }

		bool checkInside(const vec3& position);

	protected:
		Collider m_collider;
	};

	/*class TOY_CORE_EXPORT GridBoundArea
	{
	public:
		GridBoundArea(const vec3& origin, float gridWidth, float gridLength, const std::vector<int>& indices);
		bool checkInside(const vec3& position);
	};*/
}
