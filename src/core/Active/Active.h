//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <obj/Util/Updatable.h>
#include <core/Generated/Forward.h>
#include <core/Active/Effect.h>

#include <vector>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT State
	{
	public:
		State(Active& active, const string& name, int priority, float weight);

		Active* m_active;
		string m_name;
		int m_priority;
		float m_weight;
	};

	class _refl_ TOY_CORE_EXPORT Active : public NonCopy, public Updatable//, public EffectCallback
    {
	public:
		_constr_ Active(Entity& entity);
		~Active();

		_attr_ Entity& m_entity;

		std::vector<Effect> m_effects;
		std::vector<State> m_states;

		void addEffect(const string& name, float duration = 0.f, bool loop = true);
		void removeEffect(const string& name);

		void addState(const string& name, int priority, float weight);
		void removeState(const string& name);

		void next_frame(size_t tick, size_t delta);
    };
}
