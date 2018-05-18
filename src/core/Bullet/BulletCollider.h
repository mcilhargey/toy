//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <core/Store/Array.h>
#include <obj/Util/Dispatch.h>
#include <core/Generated/Forward.h>
#include <core/Physic/Collider.h>
#include <core/Physic/CollisionShape.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT BulletShape : public Movabl
	{
	public:
		BulletShape(unique_ptr<btCollisionShape> shape);
		BulletShape(unique_ptr<btCollisionShape> shape, unique_ptr<btStridingMeshInterface> mesh);
		~BulletShape();

		BulletShape(BulletShape&& other);
		BulletShape& operator=(BulletShape&& other);

		unique_ptr<btCollisionShape> shape;
		unique_ptr<btStridingMeshInterface> mesh;
	};

	class TOY_CORE_EXPORT DispatchBulletShape : public Dispatch<BulletShape>, public LazyGlobal<DispatchBulletShape>
	{
	public:
		DispatchBulletShape();
		BulletShape dispatch(CollisionShape& collisionShape);
	};

    class _refl_ TOY_CORE_EXPORT BulletCollider : public NonCopy, public ColliderImpl
    {
	public:
        BulletCollider(SubBulletWorld& bulletWorld, Collider& collider, bool create = true);
        ~BulletCollider();

		void setupObject(unique_ptr<btCollisionObject> collisionObject);

		virtual void updateTransform(const vec3& position, const quat& rotation) override;
		virtual void forceUpdate() override;

		virtual void project(const vec3& position, ContactList& collisions, short int mask) override;
		virtual void raycast(Collider& other, ContactList& obstacles, short int mask) override;

		virtual void setForce(const vec3& force) override { UNUSED(force); }
		virtual void setTorque(const vec3& torque) override { UNUSED(torque); }

	public:
		SubBulletWorld& m_bulletWorld;
		Collider& m_collider;
		BulletShape m_collisionShape;
		unique_ptr<btCollisionObject> m_collisionObject;
    };
}
