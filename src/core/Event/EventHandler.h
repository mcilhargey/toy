//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>

/* std */
#include <functional>

using namespace mud; namespace toy
{
	class EventHandler_
	{
	public:
		virtual void handleEvent(Event& event) = 0;
	};

	template <class T_Event>
	class TEventHandler : public EventHandler_
	{
	public:
		typedef std::function<void (T_Event&)> Handler;

	public:
		TEventHandler(const Handler& handler) : m_handler(handler) {}

		void handleEvent(Event& event) { m_handler(static_cast<T_Event&>(event)); }

	private:
		Handler m_handler;
	};
}
