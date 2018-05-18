//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <core/Generated/Forward.h>
#include <core/Physic/CollisionGroup.h>
#include <core/Physic/Collider.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT ObstacleBody : public Collider, public ColliderObject
	{
	public:
		ObstacleBody(Entity& entity, Medium& medium, const CollisionShape& shape, float throughput);

		_attr_ inline CollisionShape& collisionShape() { return m_collisionShape; }
		_attr_ _mut_ float m_throughput;
	};

	class _refl_ TOY_CORE_EXPORT Obstacle : public NonCopy
	{
	public:
		_constr_ Obstacle(Entity& entity, const CollisionShape& shape);
		~Obstacle();

		_attr_ Entity& m_entity;
		_attr_ CollisionShape m_shape;

		void addBody(Medium& medium, float throughput);
		void addBody(Medium& medium, const CollisionShape& shape, float throughput);

	protected:
		std::vector<object_ptr<ObstacleBody>> m_bodies;
	};
}
