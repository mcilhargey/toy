//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Active/Active.h>

#include <math/Timer.h>
#include <core/Active/Effect.h>
#include <core/Entity/Entity.h>
#include <core/World/World.h>

using namespace mud; namespace toy
{
	template <class T>
	inline void vector_remove_named(std::vector<T>& vector, const string& name)
	{
		vector.erase(std::remove_if(vector.begin(), vector.end(), [=](T& obj) { return obj.m_name == name; }), vector.end());
	}

	State::State(Active& active, const string& name, int priority, float weight)
		: m_active(&active)
		, m_name(name)
		, m_priority(priority)
		, m_weight(weight)
	{}

    Active::Active(Entity& entity)
		: m_entity(entity)
    {
		entity.m_world.add_task(this, 1);
	}

    Active::~Active()
    {
		m_entity.m_world.remove_task(this, 1);
	}

	void Active::addState(const string& name, int priority, float weight)
	{
		m_states.push_back(State{ *this, name, priority, weight });
	}

	void Active::removeState(const string& name)
	{
		vector_remove_named(m_states, name);
	}

	void Active::addEffect(const string& name, float duration, bool loop)
	{
		m_effects.push_back(Effect{ *this, name, duration, loop });
	}

	void Active::removeEffect(const string& name)
	{
		Effect* effect = vector_find(m_effects, [=](Effect& effect) { return effect.m_name == name; });
		if(effect)
			effect->stop();
	}

	void Active::next_frame(size_t tick, size_t delta)
	{
		auto func = [tick, delta](Effect& effect) { return !effect.update(tick, (delta * c_tick_interval)); };
		Effect* effect = vector_find(m_effects, func);
		while(effect)
		{
			vector_remove_if(m_effects, [=](Effect& look) { return effect == &look; });
			effect = vector_find(m_effects, func);
		}
	}
}
