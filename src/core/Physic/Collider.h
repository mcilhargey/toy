//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Util/Updatable.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>
#include <core/Physic/CollisionGroup.h>
#include <core/Physic/CollisionShape.h>
#include <core/Entity/EntityObserver.h>
#include <core/Movable/MotionState.h>

/* std */
#include <memory>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT ColliderImpl
	{
	public:
		typedef std::vector<Collider*> ContactList;

	public:
		virtual ~ColliderImpl() {}

        //virtual void update() = 0;
		virtual void forceUpdate() = 0;

		virtual void updateTransform(const vec3& position, const quat& rotation) = 0;

		virtual void project(const vec3& position, ContactList& collisions, short int mask) = 0;
		virtual void raycast(Collider& other, ContactList& obstacles, short int mask) = 0;

		virtual void setForce(const vec3& force) = 0;
		virtual void setTorque(const vec3& torque) = 0;
	};

	class _refl_ TOY_CORE_EXPORT ColliderObject
	{
	public:
		virtual ~ColliderObject() {}
		virtual void addContact(Collider& object) { UNUSED(object); }
		virtual void removeContact(Collider& object) { UNUSED(object); }
	};

    class _refl_ TOY_CORE_EXPORT Collider : public NonCopy, public Updatable, public HookObserver, public MotionSource
    {
	public:
		typedef std::vector<Collider*> ContactList;

	public:
        Collider(Entity& entity, ColliderObject& object, const CollisionShape& collisionShape, Medium& medium, CollisionGroup group, bool init = true);
        virtual ~Collider();

		Entity& m_entity;
		ColliderObject& m_object;
		CollisionShape m_collisionShape;
		Medium& m_medium;
		CollisionGroup m_group;

		SubPhysicWorld& m_world;
		object_ptr<ColliderImpl> m_impl;

		MotionState m_motionState;

		void init();

		void next_frame(size_t tick, size_t delta);

		void forceUpdate();

		virtual void updateTransform(const vec3& position, const quat& rotation);
		virtual void handleMoved() {}
		
		void project(const vec3& position, ContactList& collisions, short int mask);
		void raycast(Collider& other, ContactList& obstacles, short int mask);

		virtual void unhooked();
		virtual void hooked();
    };

	class _refl_ TOY_CORE_EXPORT Solid : public Collider
	{
	public:
		Solid(Entity& entity, ColliderObject& object, const CollisionShape& collisionShape, Medium& medium, CollisionGroup group, bool isstatic, float mass = 0.f);
		~Solid();

		bool m_static;
		float m_mass;

		virtual void unhooked();
		virtual void hooked();
	};
}
