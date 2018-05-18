//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Bullet/BulletSolid.h>

#define TOY_PRIVATE
#include <core/Bullet.h>

#include <core/Entity/Entity.h>
#include <core/Movable/Movable.h>
#include <core/Bullet/BulletWorld.h>
#include <core/Bullet/BulletMotionState.h>

/* bullet */

#if _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4127)
#	pragma warning (disable : 4100)
#endif

#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>

#if _MSC_VER
#	pragma warning (pop)
#endif

using namespace mud; namespace toy
{
    BulletSolid::BulletSolid(SubBulletWorld& bulletWorld, Solid& solid)
        : BulletCollider(bulletWorld, solid)
		, m_rigidBody(nullptr)
        , m_motionState(make_unique<BulletMotionState>(solid.m_motionState, solid.m_collisionShape.m_center))
    {
		btVector3 inertia;
		if(!solid.m_static)
			m_collisionShape.shape->calculateLocalInertia(solid.m_mass, inertia);

		this->setupObject(make_unique<btRigidBody>(solid.m_mass, m_motionState.get(), m_collisionShape.shape.get(), inertia));

		m_collisionObject->setWorldTransform(btTransform(to_btquat(m_collider.m_entity.absoluteRotation()), to_btvec3(m_collider.m_entity.absolutePosition())));

		m_rigidBody = &static_cast<btRigidBody&>(*m_collisionObject);

		if(solid.m_static)
		{
			m_rigidBody->setContactProcessingThreshold(0.02f);
			m_rigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
		}
    }

	BulletSolid::~BulletSolid()
    {}

	void BulletSolid::setForce(const vec3& force)
	{
		m_rigidBody->activate(true);
		//m_rigidBody->applyCentralForce(btvec(force));
		m_rigidBody->applyCentralImpulse(to_btvec3(force));
	}

	void BulletSolid::setTorque(const vec3& torque)
	{
		m_rigidBody->activate(true);
		m_rigidBody->setAngularVelocity(to_btvec3(torque));
	}

	void BulletSolid::updateTransform(const vec3& position, const quat& rotation)
	{
		m_rigidBody->activate();
		m_collisionObject->setWorldTransform(btTransform(to_btquat(rotation), to_btvec3(position)));
	}
}
