//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <core/Forward.h>
#include <core/Event/EventTube.h>

#ifndef MUD_CPP_20
#include <set>
#include <memory>
#endif

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT EventRelay : public NonCopy
    {
	public:
		constr_ EventRelay(Entity& entity);

		attr_ Entity& m_entity;

		void subscribeEvent(EventSubscriber& subscriber);
		void subscribeReceived(EventSubscriber& subscriber);
		void sendEvent(Event& evt);

		void appendEventTube(EventTube& tube);
		void removeEventTube(EventTube& tube);

		void appendEventTubeEnd(EventTubeEnd& tube);
		void removeEventTubeEnd(EventTubeEnd& tube);

    protected:
		EventTube m_abstractEventTube;

		std::set<EventTube*> m_eventTubes;
		std::set<EventTubeEnd*> m_eventTubeEnds;
		std::set<EventSubscriber*> m_receivedSubscribers;
    };
}
