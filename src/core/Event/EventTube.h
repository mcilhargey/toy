//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <infra/NonCopy.h>
#include <obj/Unique.h>
#include <core/Forward.h>

#ifndef MUD_CPP_20
#include <set>
#include <map>
#include <string>
#endif

using namespace mud; namespace toy
{		
	using string = std::string;

	// @ add a Filler discriminating between event types
	class TOY_CORE_EXPORT EventTube : public NonCopy
	{
	public:
		EventTube();
		~EventTube();

		void subscribe(EventSubscriber& subscriber);
		void unsubscribe(EventSubscriber& subscriber);
		void dispatch(Event& event);

	protected:
		std::set<EventSubscriber*> m_subscribers;
		std::map<string, object_ptr<EventFilter>> m_filters;
	};

	class TOY_CORE_EXPORT EventTubeEnd
	{
	public:
		EventTubeEnd();

		void subscribe(EventSubscriber& subscriber);
		void unsubscribe(EventSubscriber& subscriber);
		void bind(EventTube& eventTube);
		void unbind(EventTube& eventTube);

	protected:
		std::set<EventSubscriber*> m_subscribers;
	};

}
