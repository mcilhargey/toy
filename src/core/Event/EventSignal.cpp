//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Types.h>
#include <core/Event/EventSignal.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Medium.h>
#include <core/Physic/CollisionShape.h>

#include <core/Event/EventTube.h>
#include <core/Event/EventRelay.h>

using namespace mud; namespace toy
{
	EventEmitter::EventEmitter(Entity& entity, EventRelay& event_relay)
		: Emitter(entity)
	{
		event_relay.appendEventTube(*this);
	}

	EventReceptor::EventReceptor(Entity& entity, EventRelay& event_relay)
		: Receptor(entity)
		, EventTubeEnd()
	{
		event_relay.appendEventTubeEnd(*this);
	}

	void EventReceptor::add_scope(Medium& medium, float radius, CollisionGroup group)
	{
		ReceptorSphere& scope = this->add_sphere(medium, radius, group);
		scope.m_scope.observe(*this);
	}

	void EventReceptor::removeScope(ReceptorScope& scope)
	{
		scope.m_scope.unobserve(*this);
	}

	void EventReceptor::handle_add(Entity& entity)
	{
		this->bind(entity.as<EventEmitter>());
	}

	void EventReceptor::handle_remove(Entity& entity)
	{
		this->unbind(entity.as<EventEmitter>());
	}
}
