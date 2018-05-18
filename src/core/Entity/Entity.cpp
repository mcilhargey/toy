//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
/* toy */

#include <core/Entity/Entity.h>

#include <obj/Util/Timer.h>
#include <math/Math.h>

#include <core/World/World.h>

#include <core/Entity/EntityObserver.h>
#include <core/World/Section.h>

#include <obj/Proto.h>

#include <obj/Reflect/Meta.h>

using namespace mud; namespace toy
{
	Entity::Entity(Id id, Prototype& prototype, World& world, Entity* parent, const vec3& position, const quat& rotation)
        : Complex(id, type<Entity>(), prototype)
		, m_world(world)
		, m_parent(parent)
		, m_position(position)
		, m_rotation(rotation)
		, m_hooked(true)
	{
		world.addTask(this, short(Task::Entity)); // @todo in the long term this should be moved out of the entity's responsibility
	}

	Entity::Entity(Id id, Prototype& prototype, Entity& parent, const vec3& position, const quat& rotation)
		: Entity(id, prototype, parent.m_world, &parent, position, rotation)
	{}

    Entity::~Entity()
    {
		m_world.removeTask(this, short(Task::Entity));

		if(m_parent)
			m_parent->m_contents.remove(*this);

		m_parent = nullptr;
		
		for(Entity* entity : m_contents.store())
			entity->m_parent = nullptr;
	}

	Entity& Entity::origin()
	{
		return m_world.origin();
	}

	void Entity::debugContents(size_t depth)
	{
		if(m_contents.store().size() > 1)
		{
			for(size_t o = 0; o < depth; ++o)
				printf("    ");

			printf("Entity %u : %zu leafs\n", m_id, m_contents.store().size());
		}

		for(auto& child : m_contents.store())
			child->debugContents(depth + 1);
	}

	vec3 Entity::absolutePosition()
	{
		if(m_parent)
			return m_position + m_parent->absolutePosition();
		else
			return m_position;
	}

	quat Entity::absoluteRotation()
	{
		if(m_parent)
			return m_rotation * m_parent->absoluteRotation();
		else
			return m_rotation;
	}

	void Entity::translate(const vec3& vec)
	{
		setPosition(mud::rotate(m_rotation, vec) + m_position);
	}

	void Entity::rotate(const vec3& axis, float angle)
	{
		quat rot = angle_axis(angle, mud::rotate(m_rotation, axis));
		setRotation(rot * m_rotation);
		normalize(m_rotation);
	}

	void Entity::yaw(float value)
	{
		vec3 axis(glm::rotate(m_rotation, Y3));			
		rotate(axis, value);
	}

	void Entity::fixedYaw(float value)
	{			
		rotate(Y3, value);
	}

	void Entity::pitch(float value)
	{
		vec3 axis(glm::rotate(m_rotation, X3));
		rotate(axis, value);
	}

	void Entity::roll(float value)
	{
		vec3 axis(glm::rotate(m_rotation, Z3));
		rotate(axis, value);
	}

    void Entity::next_frame(size_t tick, size_t delta)
    {
		UNUSED(delta);

		m_moved = false;
		m_lastTick = tick;
	}

	void Entity::addHookObserver(HookObserver& obs)
	{
		m_hookObservers.push_back(&obs);
		if(m_hooked)
			obs.hooked();
	}

	void Entity::removeHookObserver(HookObserver& observer)
	{
		vector_remove(m_hookObservers, &observer);
	}

	void Entity::hook()
	{
		m_hooked = true;

		for(HookObserver* obs : m_hookObservers)
			obs->hooked();
	}

	void Entity::unhook()
	{
		for(HookObserver* obs : m_hookObservers)
			obs->unhooked();

		m_hooked = false;
	}

	void Entity::setParent(Entity* parent)
	{
		this->detachto(*parent);
	}

	bool Entity::isChildOf(Entity& entity)
	{
		if(m_parent == &entity)
			return true;
		else if(m_parent)
			return m_parent->isChildOf(entity);
		else
			return false;
	}

	Entity* Entity::spatialRoot()
	{
		if(m_parent)
			return m_parent->spatialRoot();
		else
			return this;
	}

	Entity* Entity::findParent(Type& part_type)
	{
		if(m_parent && m_parent->has_part(part_type))
			return m_parent;
		else if(m_parent)
			return m_parent->findParent(part_type);
		else
			return nullptr;
	}

	void Entity::detachto(Entity& moveto)
	{
		Entity& movefrom = *m_parent; 
		m_parent = &moveto;
		movefrom.m_contents.remove(*this);
		moveto.m_contents.add(*this);
		this->setDirty(false);
	}

	void Entity::detach(Entity& child)
	{
		child.m_parent = 0;
		m_contents.remove(child);
	}

	void Entity::visit(const Visitor& visitor)
	{
		if(!visitor(*this))
			return;

		for(Entity* entity : m_contents.store())
			entity->visit(visitor);
	}
}
