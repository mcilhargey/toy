//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <space/Generated/Forward.h>
#include <toy/toy.h>

/* std */
#include <array>

using namespace mud;
using namespace toy;

#ifndef MUD_GENERATOR_SKIP_INCLUDES
namespace glm
{
	inline bool operator<(const uvec2& lhs, const uvec2& rhs)
	{
		return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
	}
}
#endif

_EX_SPACE_EXPORT void ex_space_init(GameShell& app, Game& game);
_EX_SPACE_EXPORT void ex_space_pump(GameShell& app, Game& game, Widget& parent, Dockbar& dockbar);

struct _refl_ _EX_SPACE_EXPORT SpatialPower
{
	SpatialPower() : m_values{ 0.f } {}
	SpatialPower(std::array<float, 8> values) : m_values(values) {}

	std::array<float, 8> m_values;

	SpatialPower& operator+=(const SpatialPower& rhs) { for(size_t i = 0; i < 8; ++i) m_values[i] += rhs.m_values[i]; return *this; }
	SpatialPower& operator*=(float factor) { for(size_t i = 0; i < 8; ++i) m_values[i] *= factor; return *this; }
	SpatialPower operator*(float factor) { SpatialPower result; for(size_t i = 0; i < 8; ++i) result.m_values[i] *= factor; return result; }
};

class _refl_ _EX_SPACE_EXPORT Star : public Construct, public Updatable
{
public:
	_constr_ Star(Id id, Entity& parent, const vec3& position, const uvec2& coord, const std::string& name);
	~Star();

	_comp_ _attr_ Entity m_entity;
	_comp_ Emitter m_emitter;
	_comp_ Receptor m_receptor;
	_comp_ Active m_active;

	_attr_ _mut_ uvec2 m_coord;
	_attr_ _mut_ std::string m_name;
	_attr_ _mut_ int m_minerals = 0;
	_attr_ _mut_ int m_andrium = 0;

	_attr_ _mut_ Commander* m_commander = nullptr;

	Galaxy& galaxy();

	void next_frame(size_t tick, size_t delta);
};

enum class _refl_ FleetStance : unsigned int
{
	Movement,
	SpatialAttack,
	SpatialDefense,
	PlanetaryAttack,
	PlanetaryDefense
};

struct _refl_ _EX_SPACE_EXPORT FleetJump
{
	FleetJump() : m_state(DONE) {}
	FleetJump(uvec2 dest, size_t tick) : m_destination(dest), m_state(START), m_state_updated(tick) {}

	enum State : unsigned int
	{
		DONE,
		START,
		JUMP,
		END,
	};

	void update(Fleet& fleet, size_t tick);

	_attr_ uvec2 m_destination;
	State m_state;
	size_t m_state_updated = 0;
};

class _refl_ _EX_SPACE_EXPORT Fleet : public Construct, public Updatable
{
public:
	_constr_ Fleet(Id id, Entity& parent, const vec3& position, Commander& commander, const uvec2& coord, const std::string& name);
	~Fleet();

	_comp_ _attr_ Entity m_entity;
	_comp_ Emitter m_emitter;
	_comp_ Receptor m_receptor;
	_comp_ Active m_active;

	_attr_ _mut_ Commander* m_commander;
	_attr_ _mut_ uvec2 m_coord;
	_attr_ _mut_ std::string m_name;
	_attr_ _mut_ float m_experience = 0.f;
	SpatialPower m_spatial;
	_attr_ _mut_ float m_spatial_power = 0.f;
	_attr_ _mut_ float m_planetary = 0.f;
	_attr_ _mut_ vec4 m_weapon_ratio = Zero4;
	_attr_ _mut_ size_t m_speed = 0;
	_attr_ _mut_ size_t m_scan = 0;
	_attr_ _mut_ FleetStance m_stance = FleetStance::Movement;
	_attr_ _mut_ FleetJump m_jump;

	_attr_ size_t m_composition_updated = 0;

	/*_attr_ _mut_*/ std::map<ShipSchema*, size_t> m_composition;

	SpatialCombat* m_combat = nullptr;
	vec3 m_combat_direction;

	Galaxy& galaxy();

	void set_ships(ShipSchema& schema, size_t number);
	void set_ships(const std::string& code, size_t number);

	void update_stats();

	void jump();

	_meth_ void order_jump(vec2 coord, FleetStance stance);
	_meth_ void order_attack(Star& star);

	void next_frame(size_t tick, size_t delta);
};

enum class _refl_ WeaponType : unsigned int
{
	Ion = 0,
	Laser = 1,
	Plasma = 2,
	Torpedo = 3,
	Bomb = 4
};

static const WeaponType s_weapon_types[4] = { WeaponType::Ion, WeaponType::Laser, WeaponType::Plasma, WeaponType::Torpedo };

class _refl_ _EX_SPACE_EXPORT ShipHull
{
public:
};

class _refl_ _EX_SPACE_EXPORT ShipEngine
{
public:
};

class _refl_ _EX_SPACE_EXPORT ShipComponent
{
public:
};

struct _refl_  _EX_SPACE_EXPORT ShipSchema
{
public:
	ShipSchema() {}
	ShipSchema(size_t size, std::string code, std::string name, std::string conceptor, float level, float cost, float minerals,
		float andrium, float planetary, std::array<float, 8> spatial, float resistance, size_t speed, size_t scan, uvec4 weapon_count = {})
		: m_size(size), m_code(code), m_name(name), m_conceptor(conceptor), m_level(level), m_cost(cost), m_minerals(minerals)
		, m_andrium(andrium), m_planetary(planetary), m_spatial(spatial), m_resistance(resistance), m_speed(speed), m_scan(scan)
		, m_weapon_count(weapon_count), m_weapon_ratio(weapon_count == uvec4{} ? vec4{} : normalize(vec4(weapon_count)))
	{}

	_attr_ _mut_ size_t m_size;
	_attr_ _mut_ std::string m_code;
	_attr_ _mut_ std::string m_name;
	_attr_ _mut_ std::string m_conceptor;
	_attr_ _mut_ size_t m_level;
	_attr_ _mut_ float m_cost;
	_attr_ _mut_ float m_minerals;
	_attr_ _mut_ float m_andrium;

	_attr_ _mut_ float m_planetary;
	/*_attr_ _mut_*/ SpatialPower m_spatial;
	_attr_ _mut_ float m_resistance;
	_attr_ _mut_ size_t m_speed;
	_attr_ _mut_ size_t m_scan;

	_attr_ _mut_ uvec4 m_weapon_count;
	_attr_ _mut_ vec4 m_weapon_ratio;

	//ShipHull* m_hull;
	//ShipEngine* m_engine;
	//std::vector<ShipComponent*> m_components;
};

class _EX_SPACE_EXPORT ShipDatabase : public LazyGlobal<ShipDatabase>
{
public:
	ShipDatabase();

	ShipSchema& schema(const std::string& code);

	std::vector<ShipSchema> m_schemas;
};

class _refl_ _EX_SPACE_EXPORT Commander : public Updatable
{
public:
	_constr_ Commander(Id id, const std::string& name, int command, int commerce, int diplomacy);
	~Commander();

	//_comp_ _attr_ Entity m_entity;

	_attr_ _mut_ Id m_id;
	_attr_ _mut_ std::string m_name;
	_attr_ _mut_ int m_command;
	_attr_ _mut_ int m_commerce;
	_attr_ _mut_ int m_diplomacy;

	Colour m_colour;
	Image256 m_avatar;

	std::vector<Star*> m_stars;
	std::vector<Fleet*> m_fleets;

	struct Technology
	{
		int m_engines = 0;
		int m_eco_energy = 0;
		int m_scanners = 0;
		int m_planetary_shields = 0;
		int m_spatial_shields = 0;
		int m_laser_plasma_ions = 0;
		int m_torpedoes = 0;
		int m_bombs = 0;
		int m_hull_armor = 0;
		int m_mining = 0;
		int m_terraforming = 0;
		int m_construction = 0;
		int m_piloting = 0;
		int m_commerce = 0;
		int m_personnel = 0;
		int m_administration = 0;
		int cyber_command = 0;
		int cyber_commerce = 0;
		int cyber_diplomacy = 0;

	} m_technology;

	void next_frame(size_t tick, size_t delta);

	void take_star(Star& star);
	void take_fleet(Fleet& fleet);

	bool allied(Commander& commander) { return &commander == this; }
};

class _refl_ _EX_SPACE_EXPORT Quadrant : public Construct
{
public:
	_constr_ Quadrant(Id id, Entity& parent, const vec3& position, const uvec2& coord, float size);

	_comp_ _attr_ Entity m_entity;
	_comp_ Emitter m_emitter;
	_comp_ WorldPage m_world_page;
	_comp_ BufferPage m_buffer_page;

	_attr_ uvec2 m_coord;
	_attr_ float m_size;

	_attr_ std::vector<Star*> m_stars;
};

struct GalaxyGrid
{
	GalaxyGrid() {}

	void add_fleet(Fleet& fleet, uvec2 coord)
	{
		m_fleets[coord].push_back(&fleet);
	}

	void move_fleet(Fleet& fleet, uvec2 from, uvec2 dest)
	{
		vector_remove(m_fleets[from], &fleet);
		m_fleets[dest].push_back(&fleet);
		fleet.m_coord = dest;
	}

	std::map<uvec2, std::vector<Fleet*>> m_fleets;
	std::map<uvec2, Star*> m_stars;
};

using Flotilla = std::vector<Fleet*>;

struct _refl_ _EX_SPACE_EXPORT SpatialCombat
{
	SpatialCombat() : m_state(DONE) {}

	SpatialCombat(uvec2 coord, Flotilla flotilla_0, Flotilla flotilla_1, size_t tick)
		: m_coord(coord), m_flotilla_0(flotilla_0), m_flotilla_1(flotilla_1), m_state(APPROACH), m_state_updated(tick)
	{
		for(Fleet* fleet : m_flotilla_0)
		{
			fleet->m_combat = this;
			fleet->m_combat_direction = Z3;
		}
		for(Fleet* fleet : m_flotilla_1)
		{
			fleet->m_combat = this;
			fleet->m_combat_direction = -Z3;
		}
	}

	~SpatialCombat()
	{
		for(Fleet* fleet : m_flotilla_0)
			fleet->m_combat = nullptr;
		for(Fleet* fleet : m_flotilla_1)
			fleet->m_combat = nullptr;
	}

	enum State : unsigned int
	{
		DONE,
		APPROACH,
		ENGAGE,
	};

	void update(Fleet& fleet, size_t tick);

	void resolve();

	_attr_ uvec2 m_coord;
	_attr_ std::vector<Fleet*> m_flotilla_0;
	_attr_ std::vector<Fleet*> m_flotilla_1;
	State m_state;
	size_t m_state_updated = 0;
};

class _refl_ _EX_SPACE_EXPORT Galaxy : public Construct
{
public:
	_constr_ Galaxy(Id id, Entity& parent, const vec3& position, const uvec2& size);

	_comp_ _attr_ Entity m_entity;

	_attr_ std::vector<Quadrant*> m_quadrants;
	_attr_ std::vector<Star*> m_stars;
	_attr_ std::vector<Fleet*> m_fleets;
	_attr_ std::vector<Commander*> m_commanders;
	_attr_ uvec2 m_size;

	GalaxyGrid m_grid;

	std::vector<SpatialCombat> m_combats;

	uvec3 m_period;
	Plane m_plane;
	uvec2 m_highlight;

	uvec2 intersect_coord(Ray ray);
};

class _refl_ _EX_SPACE_EXPORT Universe : public Construct
{
public:
	_constr_ Universe(const std::string& name);
	~Universe();

	_comp_ _attr_ World m_world;
	_comp_ BulletWorld m_bulletWorld;

	std::vector<Galaxy*> m_galaxies;
};

class _refl_ _EX_SPACE_EXPORT CommanderBrush : public Brush
{
public:
	CommanderBrush(ToolContext& context);

	_attr_ _mut_ Commander* m_commander;
	_attr_ _mut_ float m_radius;

	virtual ToolState start();
	virtual void update(const vec3& position);
	virtual void paint(Gnode& parent);
};

extern float g_hullWeight[8];