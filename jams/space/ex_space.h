//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <space/Forward.h>
#include <toy/toy.h>

#include <array>
#include <set>
#include <map>

using namespace mud;
using namespace toy;

extern "C"
{
_SPACE_EXPORT void ex_space_init(GameShell& app, Game& game);
_SPACE_EXPORT void ex_space_start(GameShell& app, Game& game);
_SPACE_EXPORT void ex_space_scene(GameShell& app, GameScene& game);
_SPACE_EXPORT void ex_space_pump(GameShell& app, Game& game);
}

struct Turn;
_SPACE_EXPORT void next_turn(Turn& turn, Galaxy& galaxy);

enum class refl_ GameMode : size_t
{
	Empire = 0,
	Tactics = 1,
	TurnReport = 2
};

enum class refl_ Race : unsigned int
{
	Human,
	Bulvoid,
	Techtulon,
	Rakrarior,
	Seigneur,
	Meton,
	Tissinar,
	Zwiie
};

enum class refl_ Regime : unsigned int
{
	Empire,
	Dictature,
	Democracy,
	Theocracy,
	Technocracy,
	Piratery,
};

enum class refl_ Taxation : unsigned int
{
	None,			// revenue x0, stability +6%/turn
	Light,			// revenue x1, stability +3%/turn
	Medium,			// revenue x1.5
	Heavy,			// revenue x2, stability -3%/turn, growth -1%
	VeryHeavy,		// revenue x2.5, stability -7%/turn, growth -3%
	Total,			// revenue x3, stability -12%/turn, growth -5%
};

enum class refl_ Resource : unsigned int
{
	Minerals,
	Andrium,
	Alcool,
	Slaves,
	Narcotics,
	Food,
	Medicine,
	Computers,
	Plastic,
	Robot,
	Count,
};

enum class refl_ FleetSize : unsigned int
{
	Ridicule,
	Minuscule,
	VerySmall,
	Small,
	Medium,
	Respectable,
	Grand,
	HighGrand,
	Colossal,
	Titanesque,
	Cyclopean,
	Divine,
};

enum class refl_ Experience : unsigned int
{
	Inexperienced,
	LittleExperienced,
	Experienced,
	Superior,
	Elite
};

enum class refl_ FleetStance : unsigned int
{
	Movement,			// -15% combat, +2 movement
	SpatialAttack,		// 
	SpatialDefense,		// +10% combat for first combat
	PlanetaryAttack,	// -10% spatial combat
	PlanetaryDefense,	// 
	Pillage,			// -15% spatial combat
};

enum class refl_ WeaponType : unsigned int
{
	None = 0,
	Ion = 1,
	Laser = 2,
	Plasma = 3,
	Torpedo = 4,
	Bomb = 5
};

enum class refl_ Technology : unsigned int
{
	Engines,			// engine range +1/level
	EcoEnergy,			// ships upkeep -4%/level until level 10, then -2%/level
	Scanners,			// scanner range +1/level
	PlanetaryShields,	// planetary shield +2%/level 
	SpatialShields,		// shields resistance +2%/level
	LaserPlasmaIons,	// laser/plasma/ions power +10%/level
	Torpedoes,			// torpedoes power +10%/level
	Bombs,				// bombs power +10%/level
	HullArmor,			// hull resistance +5%/level
	Mining,				// mining output +10%/level
	Terraforming,		// max star terraforming level
	Construction,		// max buildable ship level
	Piloting,			// spatial combat +5%/level
	Commerce,			// selling price +1.5%/level
	Personnel,			// buildings upkeep -3%/level
	Administration,		// income +2%/level
	CyberCommand,		// commandment +3pts/level
	CyberCommerce,		// commerce +3pts/level
	CyberDiplomacy,		// diplomacy +3pts/level
	Count,
};

inline float eco_energy(int level) { int under10 = min(level, 10); int above10 = max(level - 10, 0); return float(under10) * -4.f + float(above10) * -2.f; }

enum class refl_ CombatType : unsigned int
{
	Spatial,
	Planetary
};

static const WeaponType s_weapon_types[4] = { WeaponType::Ion, WeaponType::Laser, WeaponType::Plasma, WeaponType::Torpedo };

extern float g_hullWeight[8];

struct RacialFactors
{
	float m_growth;
	float m_research;
	float m_mining;
	float m_planetary_combat;
	float m_spatial_combat;
	int m_command;
	int m_commerce;
	int m_diplomacy;
};

const RacialFactors c_race_factors[8] =
{
//    growth	research	mining	planetary	spatial		cmd  dipl com
	{ 1.f,		1.f,		1.f,	1.f,		1.f,		  0, +10, +10 },
	{ 0.98f,	1.05f,		1.f,	1.f,		1.f,		  0,   0,   0 },
	{ 0.95f,	1.15f,		0.95f,	0.85f,		0.90f,		  0, +10, +10 },
	{ 1.05f,	0.75f,		0.95f,	1.20f,		1.05f,		+10, -10, -10 },
	{ 0.90f,	1.f,		1.f,	1.f,		1.f,		+10,   0, -10 },
	{ 1.10f,	1.f,		1.1f,	0.85f,		0.85f,		 -5,  +5,  +5 },
	{ 1.f,		0.95f,		0.95f,	0.95f,		1.f,		  0,   0,   0 },
	{ 1.5f,		0.8f,		0.8f,	1.20f,		1.f,		  0, -10, -10 },
};

inline FleetSize fleet_size(float power)
{
	if(power < 100.f)			return FleetSize::Ridicule;
	else if(power < 300.f)		return FleetSize::Minuscule;
	else if(power < 800.f)		return FleetSize::VerySmall;
	else if(power < 1'600.f)	return FleetSize::Small;
	else if(power < 3'200.f)	return FleetSize::Medium;
	else if(power < 7'500.f)	return FleetSize::Respectable;
	else if(power < 15'000.f)	return FleetSize::Grand;
	else if(power < 30'000.f)	return FleetSize::HighGrand;
	else if(power < 60'000.f)	return FleetSize::Colossal;
	else if(power < 125'000.f)	return FleetSize::Titanesque;
	else if(power < 250'000.f)	return FleetSize::Cyclopean;
	else						return FleetSize::Divine;
}

inline Experience fleet_experience(float exp)
{
	if(exp < 5.f) return Experience::Inexperienced;
	else if(exp < 20.f) return Experience::LittleExperienced;
	else if(exp < 40.f) return Experience::Experienced;
	else if(exp < 70.f) return Experience::Superior;
	else return Experience::Elite;

}

inline int construction_time(int level)
{
	if(level < 3) return 1;
	else if(level < 5) return 2;
	else if(level < 6) return 3;
	else if(level < 7) return 4;
	else if(level < 8) return 5;
	else return 6;
}

inline float star_revenue(Taxation taxation)
{
	if(taxation == Taxation::None) return 0.f;
	else if(taxation == Taxation::Light) return 1.f;
	else if(taxation == Taxation::Medium) return 1.5f;
	else if(taxation == Taxation::Heavy) return 2.f;
	else if(taxation == Taxation::VeryHeavy) return 2.5f;
	else if(taxation == Taxation::Total) return 3.f;
	return 0.f;
}

inline float fleet_stance_modifier(FleetStance stance, CombatType combat, bool fought)
{
	if(stance == FleetStance::Movement) return -15.f;
	else if(stance == FleetStance::SpatialAttack) return 0.f;
	else if(stance == FleetStance::SpatialDefense) return fought ? 0.f : 10.f;
	else if(stance == FleetStance::PlanetaryAttack) return combat == CombatType::Spatial ? -10.f : 0.f;
	else if(stance == FleetStance::PlanetaryDefense) return 0.f;
	else if(stance == FleetStance::Pillage) return combat == CombatType::Spatial ? -15.f : 0.f;
	return 0.f;
}

inline int techno_level(int points)
{
	if(points >= 130'000) return 18;
	else if(points >= 115'000) return 17;
	else if(points >= 100'000) return 16;
	else if(points >= 85'000) return 15;
	else if(points >= 70'000) return 14;
	else if(points >= 60'000) return 13;
	else if(points >= 50'000) return 12;
	else if(points >= 40'000) return 11;
	else if(points >= 30'000) return 10;
	else if(points >= 20'000) return 9;
	else if(points >= 10'000) return 8;
	else if(points >= 6'000) return 7;
	else if(points >= 2'400) return 6;
	else if(points >= 1'200) return 5;
	else if(points >= 600) return 4;
	else if(points >= 400) return 3;
	else if(points >= 200) return 2;
	else if(points >= 100) return 1;
	else return 0;
}

struct VisuPlanet
{
	VisuPlanet(vec2 ellipsis, quat rotation, float offset, float speed) : m_ellipsis(ellipsis), m_rotation(rotation), m_speed(speed), m_period(offset) {}
	vec2 m_ellipsis;
	quat m_rotation;
	float m_speed;
	float m_period = 0.f;
};

struct VisuStar
{
	Commander* m_commander = nullptr;
	std::vector<VisuPlanet> m_planets;
	float m_period = 0.f;
	size_t m_updated = 0;
};

struct WeaponRay
{
	vec3 m_start;
	vec3 m_end;
	vec3 m_dir;
	float m_length;
	quat m_angle;
	float m_d;
};

struct VisuShip
{
	float m_radius;
	vec3 m_spot;
	float m_depth;

	vec3 m_position;

	WeaponType m_weapon;
	float m_cooldown;
	WeaponRay m_ray;

	bool m_destroyed;
};

struct VisuFleet
{
	std::vector<VisuShip> m_ships[8];
	Points m_points;
	float m_angle = 0.f;
	float m_offset = 0.f;
	vec3 m_dilate = Unit3;
	size_t m_updated = 0;
};

enum class Stage : unsigned int
{
	Divisions = 0,
	Movements = 1,
	Combats = 2,
	Systems = 3,
	Constructions = 4,
	Revolts = 5,
	Technology = 6,
	Payments = 7,
	Count = 8
};

struct TurnEvents
{
	struct Item
	{
		std::string m_summary;
	};

	enum_array<Stage, std::vector<Item>> m_items;
};

struct Turn
{
	Stage m_stage = Stage::Divisions;

	void next_state() { m_stage = Stage(uint(m_stage) + 1); }

	void add_item(Stage stage, Commander& commander, std::string summary)
	{
		m_events[&commander].m_items[size_t(stage)].push_back({ summary });
	}

	std::map<Commander*, TurnEvents> m_events;

	std::vector<SpatialCombat*> m_spatial_combats;
	std::vector<PlanetaryCombat> m_planetary_combats;
};

class refl_ _SPACE_EXPORT Player
{
public:
	Player(Galaxy* galaxy, Commander* commander) : m_galaxy(galaxy), m_commander(commander) {}
	Colour col;
	Galaxy* m_galaxy;
	Commander* m_commander;

	GameMode m_mode = GameMode::Empire;
	
	Ref m_selected_item = {};
	Ref m_hovered_item = {};

	Turn m_last_turn;
	Turn m_turn_replay;

	SpatialCombat* m_current_combat;
};

struct refl_ _SPACE_EXPORT SpatialPower
{
	SpatialPower() : m_values{ 0.f } {}
	SpatialPower(std::array<float, 8> values) : m_values(values) {}

	std::array<float, 8> m_values;

	SpatialPower& operator+=(const SpatialPower& rhs) { for(size_t i = 0; i < 8; ++i) m_values[i] += rhs.m_values[i]; return *this; }
	SpatialPower& operator*=(float factor) { for(size_t i = 0; i < 8; ++i) m_values[i] *= factor; return *this; }
	SpatialPower operator*(float factor) { SpatialPower result; for(size_t i = 0; i < 8; ++i) result.m_values[i] *= factor; return result; }
};

struct refl_ _SPACE_EXPORT Construction
{
	Schema* m_schema;
	int m_number;
	Fleet* m_destination;
	int m_turns;
};

class refl_ _SPACE_EXPORT Star : public Complex, public Updatable
{
public:
	constr_ Star(Id id, Entity& parent, const vec3& position, const uvec2& coord, const std::string& name);
	~Star();

	comp_ attr_ Entity m_entity;
	comp_ attr_ Emitter m_emitter;
	comp_ attr_ Receptor m_receptor;
	comp_ attr_ Active m_active;

	attr_ mut_ uvec2 m_coord;
	attr_ mut_ std::string m_name;
	
	enum_array<Resource, int> m_resources;
	enum_array<Resource, int> m_stocks;

	enum_array<Resource, int> m_extractors;

	std::map<BuildingSchema*, int> m_buildings;

	std::vector<Construction> m_constructions;

	attr_ mut_ int m_stability = 100;
	attr_ mut_ bool m_revolt = false;

	attr_ mut_ int m_environment = 10;
	attr_ mut_ int m_terraformation = 0;

	attr_ mut_ int m_base_population = 0;
	attr_ mut_ int m_max_population = 0;
	attr_ mut_ int m_population = 0;

	attr_ mut_ Taxation m_taxation = Taxation::Medium;

	attr_ mut_ Commander* m_commander = nullptr;

	attr_ int m_scan = 0;

	VisuStar m_visu;

	Galaxy& galaxy();

	void next_frame(size_t tick, size_t delta);

	void add_construction(Schema& schema, int number, Fleet* destination = nullptr);
};

struct refl_ _SPACE_EXPORT FleetJump
{
	FleetJump() : m_state(DONE) {}
	FleetJump(uvec2 dest, FleetStance stance, size_t tick) : m_destination(dest), m_stance(stance), m_state(START), m_state_updated(tick) {}

	enum State : unsigned int
	{
		DONE,
		START,
		JUMP,
		END,
	};

	void update(Fleet& fleet, size_t tick);

	attr_ uvec2 m_destination;
	attr_ FleetStance m_stance;
	State m_state;
	size_t m_state_updated = 0;
};

struct refl_ _SPACE_EXPORT FleetSplit
{
	FleetSplit() : m_state(DONE) {}
	FleetSplit(const std::string& name, FleetStance stance, std::map<ShipSchema*, size_t> ships, size_t tick) : m_name(name), m_stance(stance), m_ships(ships), m_state(START), m_state_updated(tick) {}

	enum State : unsigned int
	{
		DONE,
		START,
		JUMP,
		END,
	};

	void update(Fleet& fleet, size_t tick);

	attr_ std::string m_name;
	attr_ FleetStance m_stance;
	std::map<ShipSchema*, size_t> m_ships;
	State m_state;
	size_t m_state_updated = 0;
};

class refl_ _SPACE_EXPORT Fleet : public Complex, public Updatable
{
public:
	constr_ Fleet(Id id, Entity& parent, const vec3& position, Commander& commander, const uvec2& coord, const std::string& name);
	~Fleet();

	comp_ attr_ Entity m_entity;
	comp_ attr_ Emitter m_emitter;
	comp_ attr_ Receptor m_receptor;
	comp_ attr_ Active m_active;

	attr_ mut_ Commander* m_commander;
	attr_ mut_ uvec2 m_coord;
	attr_ mut_ std::string m_name;

	attr_ mut_ float m_experience = 0.f;
	attr_ mut_ SpatialPower m_spatial;
	attr_ mut_ float m_spatial_power = 0.f;
	attr_ mut_ float m_planetary_power = 0.f;
	attr_ mut_ size_t m_speed = 0;
	attr_ mut_ size_t m_scan = 0;
	attr_ mut_ float m_upkeep = 0.f;

	attr_ mut_ FleetStance m_stance = FleetStance::Movement;
	
	attr_ mut_ FleetJump m_jump;
	attr_ mut_ FleetSplit m_split;

	attr_ bool m_fought = false;
	
	FleetSize estimated_size() { return fleet_size(m_spatial_power + m_planetary_power); }
	Experience estimated_experience() { return fleet_experience(m_experience); }

	attr_ size_t m_ships_updated = 0;

	/*attr_ mut_*/ std::map<ShipSchema*, size_t> m_ships;

	VisuFleet m_visu;

	Galaxy& galaxy();

	vec3 base_position();

	void add_ships(ShipSchema& schema, size_t number);
	void set_ships(ShipSchema& schema, size_t number);

	void add_ships(const std::string& code, size_t number);
	void set_ships(const std::string& code, size_t number);

	void update_stats();

	void jump();

	meth_ void order_jump(vec2 coord, FleetStance stance);
	/*meth_*/ void order_split(cstring name, FleetStance stance, std::map<ShipSchema*, size_t> ships);
	meth_ void order_attack(Star& star);

	void next_frame(size_t tick, size_t delta);
};

struct refl_ _SPACE_EXPORT Schema
{
	Schema() {}
	Schema(std::string code, std::string name, std::string conceptor, size_t level, float cost, float minerals,
		   float andrium, float planetary = 0.f, std::array<float, 8> spatial = {}, float resistance = 0.f, size_t speed = 0, size_t scan = 0)
		: m_code(code), m_name(name), m_conceptor(conceptor), m_level(level), m_cost(cost), m_minerals(minerals)
		, m_andrium(andrium), m_planetary(planetary), m_spatial(spatial), m_resistance(resistance), m_speed(speed), m_scan(scan)
	{}

	attr_ mut_ std::string m_code;
	attr_ mut_ std::string m_name;
	attr_ mut_ std::string m_conceptor;

	attr_ mut_ size_t m_level = 1;

	attr_ mut_ float m_cost = 0.f;
	attr_ mut_ float m_minerals = 0.f;
	attr_ mut_ float m_andrium = 0.f;
	
	attr_ mut_ float m_planetary = 0.f;
	attr_ mut_ SpatialPower m_spatial = {};
	attr_ mut_ float m_resistance = 0.f;
	attr_ mut_ size_t m_speed = 0;
	attr_ mut_ size_t m_scan = 0;

	attr_ mut_ float m_upkeep_factor = 1.f;
};

struct refl_ _SPACE_EXPORT ShipHull : public Schema
{
	using Schema::Schema;
};

struct refl_ _SPACE_EXPORT ShipEngine : public Schema
{
	using Schema::Schema;
};

struct refl_ _SPACE_EXPORT ShipComponent : public Schema
{
	using Schema::Schema;
};

struct refl_ _SPACE_EXPORT ShipSchema : public Schema
{
	ShipSchema() {}
	ShipSchema(size_t size, std::string code, std::string name, std::string conceptor, size_t level, float cost, float minerals,
		float andrium, float planetary, std::array<float, 8> spatial, float resistance, size_t speed, size_t scan, std::array<uint, 6> weapon_count = {})
		: Schema(code, name, conceptor, level, cost, minerals, andrium, planetary, spatial, resistance, speed, scan)
		, m_size(size), m_weapon_count(weapon_count)
	{
		uint max_weapon = 0;
		for(size_t i = 0; i < 4; ++i)
			if(m_weapon_count[i] > max_weapon)
			{
				max_weapon = m_weapon_count[i];
				m_main_weapon = WeaponType(i);
			}
	}

	attr_ mut_ size_t m_size;

	std::array<uint, 6> m_weapon_count;

	attr_ mut_ WeaponType m_main_weapon = WeaponType::None;

	//ShipHull* m_hull;
	//ShipEngine* m_engine;
	//std::vector<ShipComponent*> m_components;
};

struct refl_  _SPACE_EXPORT BuildingSchema : public Schema
{
	using Schema::Schema;
};

template <class T_Schema>
class _SPACE_EXPORT SchemaDatabase
{
public:
	SchemaDatabase() {}

	T_Schema& schema(const std::string& code)
	{
		for(T_Schema& schema : m_schemas)
			if(schema.m_code == code)
				return schema;
	}

	std::vector<T_Schema> m_schemas;
};

class _SPACE_EXPORT ShipDatabase : public SchemaDatabase<ShipSchema>, public LazyGlobal<ShipDatabase>
{
public:
	ShipDatabase();
};

class _SPACE_EXPORT BuildingDatabase : public SchemaDatabase<BuildingSchema>, public LazyGlobal<BuildingDatabase>
{
public:
	BuildingDatabase();
};

struct refl_ _SPACE_EXPORT Scans
{
	std::set<Star*> m_stars;
	std::set<Fleet*> m_fleets;
	//attr_ std::vector<Star*> m_stars;
	//attr_ std::vector<Fleet*> m_fleets;
};

// float technology_upkeep(int points, )

struct refl_ _SPACE_EXPORT TechDomain
{
	int m_level = 0;
	int m_points = 0;
	float m_budget = 0.f;
};

class refl_ _SPACE_EXPORT Commander : public Updatable
{
public:
	constr_ Commander(Id id, const std::string& name, Race race, int command, int commerce, int diplomacy);
	~Commander();

	//comp_ attr_ Entity m_entity;

	attr_ mut_ Id m_id;
	attr_ mut_ std::string m_name;
	attr_ mut_ Race m_race;
	attr_ mut_ int m_command;
	attr_ mut_ int m_commerce;
	attr_ mut_ int m_diplomacy;

	Colour m_colour;
	Image256 m_avatar;

	attr_ std::vector<Star*> m_stars;
	attr_ std::vector<Fleet*> m_fleets;

	attr_ Star* m_capital = nullptr;
	attr_ Regime m_regime = Regime::Empire;

	attr_ float m_power = 0.f;
	attr_ float m_centaures = 0.f;

	attr_ Scans m_scans;
	
	enum_array<Technology, TechDomain> m_technology;

	inline int level(Technology tech) { return m_technology[size_t(tech)].m_level; }

	void next_frame(size_t tick, size_t delta);

	void update_scans();

	void take_star(Star& star);
	void take_fleet(Fleet& fleet);

	bool allied(Commander& commander) { return &commander == this; }
};

class refl_ _SPACE_EXPORT Quadrant : public Complex
{
public:
	constr_ Quadrant(Id id, Entity& parent, const vec3& position, const uvec2& coord, float size);

	comp_ attr_ Entity m_entity;
	comp_ attr_ Emitter m_emitter;
	comp_ attr_ WorldPage m_world_page;
	comp_ attr_ BufferPage m_buffer_page;

	attr_ uvec2 m_coord;
	attr_ float m_size;

	attr_ std::vector<Star*> m_stars;
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

using Ships = std::map<ShipSchema*, uint32_t>;
using Flotilla = std::vector<Fleet*>;

struct refl_ _SPACE_EXPORT Combat
{
};

struct refl_ _SPACE_EXPORT PlanetaryCombat
{

};

struct refl_ _SPACE_EXPORT CombatFleet
{
	CombatFleet(Fleet& fleet) : m_fleet(&fleet) {}
	Fleet* m_fleet;
	float m_damage = 0.f;
	Ships m_losses = {};
	std::array<uint32_t, 8> m_hull_losses = {};
};

struct refl_ _SPACE_EXPORT SpatialCombat
{
	SpatialCombat() : m_state(DONE) {}

	SpatialCombat(uvec2 coord, size_t tick)
		: m_coord(coord), m_state(APPROACH), m_state_updated(tick)
	{}

	enum State : unsigned int
	{
		DONE,
		APPROACH,
		ENGAGE,
	};

	attr_ uvec2 m_coord;

	attr_ std::vector<CombatFleet> m_allies;
	attr_ std::vector<CombatFleet> m_enemies;

	State m_state;
	size_t m_state_updated = 0;

	float m_t = 0.f;
	float m_t_damage = 0.f;
	float m_t_position = 0.f;
	float m_dt_intensity = 0.f;

	void apply_losses();
};

class refl_ _SPACE_EXPORT Galaxy : public Complex
{
public:
	constr_ Galaxy(Id id, Entity& parent, const vec3& position, const uvec2& size);

	comp_ attr_ Entity m_entity;

	attr_ std::vector<Quadrant*> m_quadrants;
	attr_ std::vector<Star*> m_stars;
	attr_ std::vector<Fleet*> m_fleets;
	attr_ std::vector<Commander*> m_commanders;
	attr_ uvec2 m_size;

	GalaxyGrid m_grid;

	std::vector<SpatialCombat> m_combats;

	uvec3 m_scale;
	Plane m_plane;
	uvec2 m_highlight;

	uvec2 intersect_coord(Ray ray);
};

class refl_ _SPACE_EXPORT Universe : public Complex
{
public:
	constr_ Universe(const std::string& name);
	~Universe();

	comp_ attr_ World m_world;
	comp_ attr_ BulletWorld m_bullet_world;

	std::vector<Galaxy*> m_galaxies;
};

class refl_ _SPACE_EXPORT CommanderBrush : public Brush
{
public:
	CommanderBrush(ToolContext& context);

	attr_ mut_ Commander* m_commander;
	attr_ mut_ float m_radius;

	virtual ToolState start();
	virtual void update(const vec3& position);
	virtual void paint(Gnode& parent);
};
