//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>

#include <string>

using namespace mud; namespace toy
{
	using string = std::string;

	class EffectCallback { public: virtual void effectEnded(Effect& effect) = 0; };

	class _refl_ TOY_CORE_EXPORT Effect
	{
	public:
		_constr_ Effect(Active& active, const string& name, float duration = 0.f, bool loop = true);

		bool update(size_t tick, double step);
		void stop();

		Active* m_active;
		string m_name;
		float m_duration;
		bool m_loop;

	protected:
		double m_currentTime;
		bool m_done;
	};
}
