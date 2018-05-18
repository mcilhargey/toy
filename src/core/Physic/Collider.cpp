//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Physic/Collider.h>

#include <core/Entity/Entity.h>
#include <core/World/Section.h>
#include <core/World/World.h>
#include <core/Physic/Medium.h>
#include <core/Physic/PhysicWorld.h>
#include <core/Physic/CollisionShape.h>

using namespace mud; namespace toy
{
    Collider::Collider(Entity& entity, ColliderObject& object, const CollisionShape& collisionShape, Medium& medium, CollisionGroup group, bool init)
        : m_entity(entity)
		, m_object(object)
		, m_collisionShape(collisionShape)
		, m_medium(medium)
		, m_group(group)
		, m_world(entity.m_world.part<PhysicWorld>().getSubWorld(m_medium))
		, m_impl()
		, m_motionState(entity, *this)
    {
		m_entity.m_world.addTask(this, short(Task::Physics));

		if(init)
		{
			m_impl = m_world.makeCollider(*this);
			this->init();
		}
		
		// @note : instant contact detection on world inclusion works only if a call to update is done after
		//	in a non delayed manner it is possible only by calling performDiscreteCollisionDetection on each item add
		//	in a delayed manner it is possible by updating on the frame after
		//this->forceUpdate();
	}

    Collider::~Collider()
    {
		m_entity.m_world.removeTask(this, short(Task::Physics));

		if(m_entity.m_hooked)
			this->unhooked();
	}

	void Collider::init()
	{
		m_entity.addHookObserver(*this);
	}

	void Collider::next_frame(size_t tick, size_t delta)
	{
		UNUSED(delta);

		m_motionState.update(tick);
	}

	void Collider::updateTransform(const vec3& position, const quat& rotation)
	{
		m_impl->updateTransform(position, rotation);
		this->handleMoved();
	}

	void Collider::forceUpdate()
	{
		m_impl->forceUpdate();
	}

	void Collider::project(const vec3& position, ContactList& collisions, short int mask)
	{
		m_impl->project(position, collisions, mask);
	}

	void Collider::raycast(Collider& other, ContactList& obstacles, short int mask)
	{
		m_impl->raycast(other, obstacles, mask);
	}

	void Collider::hooked()
	{
		m_world.addCollider(*this);
	}

	void Collider::unhooked()
	{
		m_world.removeCollider(*this);
	}

	Solid::Solid(Entity& entity, ColliderObject& object, const CollisionShape& collisionShape, Medium& medium, CollisionGroup group, bool isstatic, float mass)
		: Collider(entity, object, collisionShape, medium, group, false)
		, m_static(isstatic)
		, m_mass(mass)
	{
		m_impl = m_world.makeSolid(*this);
		this->init();
	}

	Solid::~Solid()
	{}

	void Solid::hooked()
	{
		m_world.addSolid(*this);
	}

	void Solid::unhooked()
	{
		m_world.removeSolid(*this);
	}
}
