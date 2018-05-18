//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <util/Generated/Forward.h>
#include <obj/Util/LocklessQueue.h>

/* std */
#include <functional>

using namespace mud; namespace toy
{
	class TOY_UTIL_EXPORT Scheduler
	{
	public:
		typedef std::function<void ()> ProcedureType;

	public:
		Scheduler(size_t queueSize);
		~Scheduler();

		bool scheduleAction(const ProcedureType& action);
		void processActions();

	private:
		LocklessQueue<ProcedureType> m_actions;
	};
}
