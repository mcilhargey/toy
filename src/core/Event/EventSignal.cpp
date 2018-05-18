//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Generated/Types.h>
#include <core/Event/EventSignal.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Medium.h>
#include <core/Physic/CollisionShape.h>

#include <core/Event/EventTube.h>
#include <core/Event/EventRelay.h>

using namespace mud; namespace toy
{
	EventEmitter::EventEmitter(Entity& entity)
		: Emitter(entity)
	{
		entity.part<EventRelay>().appendEventTube(*this);
	}

	EventReceptor::EventReceptor(Entity& entity)
		: Receptor(entity)
		, EventTubeEnd()
	{
		entity.part<EventRelay>().appendEventTubeEnd(*this);
	}

	void EventReceptor::addScope(Medium& medium, float radius, CollisionGroup group)
	{
		ReceptorSphere& scope = this->addSphere(medium, radius, group);
		scope.m_scope.observe(*this);
	}

	void EventReceptor::removeScope(ReceptorScope& scope)
	{
		scope.m_scope.unobserve(*this);
	}

	void EventReceptor::handleAdd(Entity& entity)
	{
		this->bind(entity.part<EventEmitter>());
	}

	void EventReceptor::handleRemove(Entity& entity)
	{
		this->unbind(entity.part<EventEmitter>());
	}
}
