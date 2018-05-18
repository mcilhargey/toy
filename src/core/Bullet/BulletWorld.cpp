//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Generated/Types.h>
#include <core/Bullet/BulletWorld.h>

#include <geom/Geom.h>
#include <geom/Shape.h>

#define TOY_PRIVATE
#include <core/Bullet.h>

#include <core/World/World.h>
#include <core/Bullet/BulletSolid.h>
#include <core/Bullet/BulletCollider.h>
#include <core/Physic/Solid.h>

#include <obj/Util/Timer.h>

#if _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4127) // members are private, so there's no risk them being accessed by the user
#endif

#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletCollisionCommon.h>

#if _MSC_VER
#	pragma warning (pop)
#endif

#ifdef TRIGGER_COLLISIONS
extern CollisionStartedCallback gCollisionStartedCallback;
extern CollisionEndedCallback gCollisionEndedCallback;
#endif

using namespace mud; namespace toy
{
	static void collisionStarted(btPersistentManifold* manifold)
	{
		Collider* col0 = (Collider*)((btCollisionObject*)manifold->getBody0())->getUserPointer();
		Collider* col1 = (Collider*)((btCollisionObject*)manifold->getBody1())->getUserPointer();

		if(&col0->m_entity == &col1->m_entity)
			return;

		// printf << "Add contact " << col0->m_entity.m_id << " : " << col1->m_entity.m_id << std::endl;
		col0->m_object.addContact(*col1);
		col1->m_object.addContact(*col0);
	}

#ifdef TRIGGER_COLLISIONS
	static void collisionEnded(btPersistentManifold* manifold)
	{
		Collider* col0 = (Collider*)((btCollisionObject*)manifold->getBody0())->getUserPointer();
		Collider* col1 = (Collider*)((btCollisionObject*)manifold->getBody1())->getUserPointer();

		if(&col0->m_entity == &col1->m_entity)
			return;

		// printf << "Remove contact " << col0->m_entity.m_id << " : " << col1->m_entity.m_id << std::endl;
		col0->m_object.removeContact(*col1);
		col1->m_object.removeContact(*col0); // @todo : replace this with buffered action (set a flag on bullet object ?) to not loop infinitely from bullet code
	}
#endif

#define BULLET_WORLD_SCALE 10000.f

    SubBulletWorld::SubBulletWorld(World& world, Medium& medium)
        : SubPhysicWorld(world, medium)
        , m_collisionConfiguration(make_unique<btDefaultCollisionConfiguration>())
        , m_collisionDispatcher(make_unique<btCollisionDispatcher>(m_collisionConfiguration.get()))
		, m_broadphaseInterface(make_unique<btAxisSweep3>(btVector3/*worldAabbMin*/(-1.f,-1.f,-1.f) * -BULLET_WORLD_SCALE, btVector3/*worldAabbMax*/(1.f, 1.f, 1.f) * BULLET_WORLD_SCALE, /*maxProxies*/32000)) // @crash btAssert(m_firstFreeHandle) is limited by this setting
		//, m_broadphaseInterface(make_unique<btDbvtBroadphase>()) // @crash btAssert(m_firstFreeHandle) is limited by this setting
		
	{
		if(medium.m_solid)
		{
			m_constraintSolver = make_unique<btSequentialImpulseConstraintSolver>();
			m_bulletWorld = make_unique<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(), m_broadphaseInterface.get(), m_constraintSolver.get(), m_collisionConfiguration.get());
			m_dynamicsWorld = static_cast<btDiscreteDynamicsWorld*>(m_bulletWorld.get());
		}
		else
		{
			m_bulletWorld = make_unique<btCollisionWorld>(m_collisionDispatcher.get(), m_broadphaseInterface.get(), m_collisionConfiguration.get());
		}
	}

    SubBulletWorld::~SubBulletWorld()
    {}

	object_ptr<ColliderImpl> SubBulletWorld::makeCollider(Collider& collider)
	{
		return make_object<BulletCollider>(*this, collider);
	}

	object_ptr<ColliderImpl> SubBulletWorld::makeSolid(Solid& solid)
	{
		return make_object<BulletSolid>(*this, solid);
	}

	void SubBulletWorld::addSolid(Solid& solid)
	{
		printf("DEBUG: Add solid to medium %s\n", m_medium.m_name.c_str());
		m_dynamicsWorld->addRigidBody(as<BulletSolid>(*solid.m_impl).m_rigidBody, solid.m_group, m_medium.mask(solid.m_group));
		m_solids.push_back(&solid);
	}

	void SubBulletWorld::removeSolid(Solid& solid)
	{
		m_dynamicsWorld->removeRigidBody(as<BulletSolid>(*solid.m_impl).m_rigidBody);
		this->removeContacts(solid);
		vector_remove(m_solids, &solid);
	}

	void SubBulletWorld::addCollider(Collider& collider)
	{
		printf("DEBUG: Add collision object to medium %s\n", m_medium.m_name.c_str());
		m_bulletWorld->addCollisionObject(as<BulletCollider>(*collider.m_impl).m_collisionObject.get(), collider.m_group, m_medium.mask(collider.m_group));
	}

	void SubBulletWorld::removeCollider(Collider& collider)
	{
		m_bulletWorld->removeCollisionObject(as<BulletCollider>(*collider.m_impl).m_collisionObject.get());
		this->removeContacts(collider);
	}

	void SubBulletWorld::removeContacts(Collider& collider)
	{
		for(int i = m_contacts.size() - 1; i >= 0; --i)
		{
			Contact& contact = *m_contacts[i];
			if(contact.m_col0 == &collider || contact.m_col1 == &collider)
				this->removeContact(contact, i);
		}
	}

	void SubBulletWorld::removeContact(Contact& contact, size_t index)
	{
#if 0 // DEBUG
		Entity& entity0 = contact.m_col0->m_entity;
		Entity& entity1 = contact.m_col1->m_entity;
		printf("Remove contact %s %u : %s %u\n", entity0.m_prototype.m_name.c_str(), entity0.m_id, entity1.m_prototype.m_name.c_str(), entity1.m_id);
#endif

		contact.m_col0->m_object.removeContact(*contact.m_col1);
		contact.m_col1->m_object.removeContact(*contact.m_col0);

		m_contacts.back()->m_index = index;
		std::swap(m_contacts[index], m_contacts.back());
		m_contacts.pop_back();

		removeContact(*contact.m_col0, *contact.m_col1);
	}

	void SubBulletWorld::updateContacts()
	{
#ifndef TRIGGER_COLLISIONS
		btManifoldArray manifoldArray;

		int numManifolds = m_bulletWorld->getDispatcher()->getNumManifolds();
		for(int i = 0; i<numManifolds; i++)
		{
			btPersistentManifold* manifold = m_bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

			int numContacts = manifold->getNumContacts();
			for(int j = 0; j<numContacts; j++)
			{
				btManifoldPoint& pt = manifold->getContactPoint(j);

				if(pt.getDistance() < 0.f)
				{
					Collider* col0 = (Collider*)((btCollisionObject*)manifold->getBody0())->getUserPointer();
					Collider* col1 = (Collider*)((btCollisionObject*)manifold->getBody1())->getUserPointer();

					Contact& contact = findContact(*col0, *col1);

					if(contact.m_tick == 0)
					{
						collisionStarted(manifold);
						contact.m_col0 = col0;
						contact.m_col1 = col1;
						contact.m_index = m_contacts.size();
						m_contacts.push_back(&contact);
					}

					contact.m_tick = m_lastTick;
				}
			}
		}

		for(int i = m_contacts.size() - 1; i >= 0; --i)
		{
			Contact& contact = *m_contacts[i];
			if(contact.m_tick < m_lastTick)
				this->removeContact(contact, i);
		}
#endif
	}

	void SubBulletWorld::removeContact(Collider& col0, Collider& col1)
	{
		m_hashContacts.erase(&col0 < &col1 ? m_hash(&col0, &col1) : m_hash(&col1, &col0));
	}

	SubBulletWorld::Contact& SubBulletWorld::findContact(Collider& col0, Collider& col1)
	{
		return m_hashContacts[&col0 < &col1 ? m_hash(&col0, &col1) : m_hash(&col1, &col0)];
	}

    // @note : this assume that we cap the framerate at 120fps, and that it shouldn't go lower than 12fps
    void SubBulletWorld::next_frame(size_t tick, size_t delta)
    {
		m_lastTick = tick;

		double timeStep = delta * TICK_INTERVAL;
		if(m_dynamicsWorld)
			m_dynamicsWorld->stepSimulation(timeStep, 3); // min(timeStep, 0.08)
		else
			m_bulletWorld->performDiscreteCollisionDetection();

		this->updateContacts();

		//for(Solid* solid : m_solids)
		//	as<BulletSolid>(*solid->m_impl).m_rigidBody->setAngularFactor(btVector3(0.f, 0.f, 0.f));
    }

	BulletWorld::BulletWorld(World& world)
		: PhysicWorld(world)
    {
#ifdef TRIGGER_COLLISIONS
		gCollisionStartedCallback = collisionStarted;
		gCollisionEndedCallback = collisionEnded;
#endif
	}

	BulletWorld::~BulletWorld()
    {}

	object_ptr<SubPhysicWorld> BulletWorld::createSubWorld(Medium& medium)
	{
		return make_object<SubBulletWorld>(m_world, medium);
	}

	vec3 BulletWorld::groundPoint(const Ray& ray)
	{
		btCollisionWorld::ClosestRayResultCallback callback(to_btvec3(ray.m_start), to_btvec3(ray.m_end));
		callback.m_collisionFilterMask = CM_GROUND;
		callback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;

		SubBulletWorld& subWorld = as<SubBulletWorld>(this->getSubWorld(SolidMedium::me()));
		subWorld.m_bulletWorld->rayTest(to_btvec3(ray.m_start), to_btvec3(ray.m_end), callback);

		return to_vec3(callback.m_hitPointWorld);
	}

}
