//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>

/* std */
#include <vector>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT Signal
	{
	public:
		Signal(EmitterScope& emitter, ReceptorScope& receptor);
		~Signal();

		EmitterScope* m_emitter;
		ReceptorScope* m_receptor;
		float m_strength;
		bool m_on;

		void update();
		void on();
		void off();

	private:
		std::vector<ObstacleBody*> m_occluding;
	};
}
