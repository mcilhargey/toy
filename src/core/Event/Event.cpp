//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Event/Event.h>

#include <core/Entity/Entity.h>
#include <core/Event/EventRelay.h>

using namespace mud; namespace toy
{
	Event::Event(Type& eventType, Entity& source)
		: m_eventType(eventType)
		, m_source(source.part<EventRelay>())
		, m_intensity()
		, m_attributes()
	{
		m_attributes["EventType"] = var(eventType);
	}

	EventSubscriber::EventSubscriber(Type& eventType, object_ptr<EventHandler_> handler)
		: m_eventType(eventType)
		, m_handler(std::move(handler))
		, m_filters()
	{
		m_filters["EventType"] = var(eventType);
	}

	void EventSubscriber::call(Event& event)
	{
		m_handler->handleEvent(event);
	}
}
