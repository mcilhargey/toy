//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <05_character/Generated/Types.h>
#include <05_character/Generated/Forward.h>
#include <toy/toy.h>

using namespace mud;
using namespace toy;

class _refl_ Human : public Construct, public Updatable, public ColliderObject
{
public:
	_constr_ Human(Id id, Entity& parent, const vec3& position, float radius, float height, const std::string& first_name, const std::string& last_name);
	~Human();

	_comp_ Entity m_entity;
	_comp_ Movable m_movable = { m_entity };
	_comp_ Manoeuvrable m_manoeuvrable;
	_comp_ Emitter m_emitter = { m_entity };
	_comp_ Receptor m_receptor = { m_entity };
	_comp_ Active m_active = { m_entity };
	_comp_ Actor m_actor = { m_entity };
	_comp_ Reactive m_reactive = { m_entity };

	_attr_ _mut_ std::string m_first_name;
	_attr_ _mut_ std::string m_last_name;

	_attr_ _mut_ float m_counter = 0.f;
	_attr_ _mut_ float m_next_change;

	struct State
	{
		std::string m_action;
		float m_action_speed;
	};

	std::vector<State> m_states;

	Entity* m_shooting = nullptr;

	std::string full_name() { return m_first_name + " " + m_last_name; }

	AutoStat<float> m_life = { 100.f, 0.f, 100.f };
	size_t m_last_tick = 0;

	unique_ptr<Solid> m_solid;

	Procedure* m_current_movement = nullptr;

	void next_frame(size_t tick, size_t delta);
	void pumpSecond();
};

class _refl_ Walk : public TypedAction<Walk>
{
public:
	Walk(toy::User* user, Entity& agent, Entity& target);

	static bool checkAgent(const Entity& agent);
	static bool checkTarget(const Entity& target);

	void begin();
	void update(size_t tick, double step);
	void abort();

	virtual void nextWaypoint();

protected:
	Manoeuvrable& m_manoeuvrable;
	float m_range;
};

void paint_human(Gnode& parent, Human& human);
