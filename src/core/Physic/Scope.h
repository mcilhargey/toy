//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Store/Array.h>
#include <core/Generated/Forward.h>

#include <core/Entity/Entity.h> // @array-include
#include <core/Physic/Collider.h>
#include <core/Physic/Signal.h>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT PhysicScope : public Collider, public ColliderObject
    {
    public:
        PhysicScope(Entity& entity, Medium& medium, const CollisionShape& collisionShape, CollisionGroup group);

		virtual void addContact(Collider& object);
		virtual void removeContact(Collider& collider);

		Array<Entity> m_scope;
	};

	class _refl_ TOY_CORE_EXPORT EmitterScope : public PhysicScope
	{
	public:
		EmitterScope(Entity& entity, Medium& medium, const CollisionShape& collisionShape, CollisionGroup group /*= CM_SOURCE*/);

		virtual void addContact(Collider& object);
		virtual void removeContact(Collider& collider);

		virtual void handleMoved();

	protected:
		std::vector<Signal> m_signals;
	};

	class _refl_ TOY_CORE_EXPORT ReceptorScope : public PhysicScope
	{
	public:
		ReceptorScope(Entity& entity, Medium& medium, const CollisionShape& collisionShape, CollisionGroup group /*= CM_RECEPTOR*/);
	};

	class _refl_ TOY_CORE_EXPORT EmitterSphere : public EmitterScope
	{
	public:
		EmitterSphere(Entity& entity, Medium& medium, CollisionGroup group, float radius);
	};

	class _refl_ TOY_CORE_EXPORT ReceptorSphere : public ReceptorScope
	{
	public:
		ReceptorSphere(Entity& entity, Medium& medium, CollisionGroup group, float radius);

		float m_radius;
	};

	class _refl_ TOY_CORE_EXPORT Emitter : public NonCopy
	{
	public:
		_constr_ Emitter(Entity& entity);
		~Emitter();

		 _attr_ Entity& m_entity;

		EmitterScope& addScope(Medium& medium, const CollisionShape& collisionShape, CollisionGroup group);
		EmitterSphere& addSphere(Medium& medium, float radius, CollisionGroup group = CM_SOURCE);

	protected:
		std::vector<object_ptr<EmitterScope>> m_emitters;
	};

	class _refl_ TOY_CORE_EXPORT Receptor : public NonCopy
	{
	public:
		_constr_ Receptor(Entity& entity);
		~Receptor();

		 _attr_ Entity& m_entity;

		ReceptorScope& addScope(object_ptr<ReceptorScope> emitter);
		ReceptorSphere& addSphere(Medium& medium, float radius, CollisionGroup group = CM_RECEPTOR);

		ReceptorScope* scope(Medium& medium);

	protected:
		std::vector<object_ptr<ReceptorScope>> m_receptors;
	};
}
