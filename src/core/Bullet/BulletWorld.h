//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Unique.h>
#include <core/Generated/Forward.h>
#include <core/Physic/PhysicWorld.h>

#include <unordered_map>

class btCollisionWorld;
class btDynamicsWorld;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btConstraintSolver;

class btCollisionShape;
class btStridingMeshInterface;
class btCollisionObject;
class btRigidBody;
class btPersistentManifold;

class btCollisionConfiguration;
class btCollisionDispatcher;

using namespace mud; namespace toy
{
#ifdef TRIGGER_COLLISIONS
	static void collisionStarted(btPersistentManifold* manifold);
	static void collisionEnded(btPersistentManifold* manifold);
#endif

	class _refl_ TOY_CORE_EXPORT SubBulletWorld : public SubPhysicWorld
	{
	public:
        SubBulletWorld(World& world, Medium& medium);
        ~SubBulletWorld();

		void updateContacts();

        void next_frame(size_t tick, size_t delta);

		virtual object_ptr<ColliderImpl> makeCollider(Collider& collider);
		virtual object_ptr<ColliderImpl> makeSolid(Solid& solid);

		void addSolid(Solid& solid);
		void removeSolid(Solid& solid);

		void addCollider(Collider& collider);
		void removeCollider(Collider& collider);

		void removeContacts(Collider& collider);

    public:
		size_t m_lastTick;
		unique_ptr<btCollisionConfiguration> m_collisionConfiguration;
        unique_ptr<btCollisionDispatcher> m_collisionDispatcher;
        unique_ptr<btBroadphaseInterface> m_broadphaseInterface;

		unique_ptr<btCollisionWorld> m_bulletWorld;
        unique_ptr<btConstraintSolver> m_constraintSolver;

		btDynamicsWorld* m_dynamicsWorld = nullptr;

		std::vector<Solid*> m_solids;

#ifndef TRIGGER_COLLISIONS
		struct pairhash
		{
			template <class T, class U>
			size_t operator()(const T& first, const U& second) const { return std::hash<T>()(first) ^ std::hash<U>()(second); }
		};

		pairhash m_hash;

		struct Contact
		{
			Contact() : m_tick(0) {}

			Collider* m_col0;
			Collider* m_col1;
			size_t m_tick;
			size_t m_index;
		};

		std::unordered_map<size_t, Contact> m_hashContacts;
		std::vector<Contact*> m_contacts;

		void removeContact(Contact& contact, size_t index);

		void removeContact(Collider& col0, Collider& col1);
		Contact& findContact(Collider& col0, Collider& col1);
#endif
	};

	class _refl_ TOY_CORE_EXPORT BulletWorld : public PhysicWorld
	{
	public:
		_constr_ BulletWorld(World& world);
        ~BulletWorld();

		object_ptr<SubPhysicWorld> createSubWorld(Medium& medium);
		vec3 groundPoint(const Ray& ray);
    };
}
