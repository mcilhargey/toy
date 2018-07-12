//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <blocks/Forward.h>
#include <toy/toy.h>

using namespace mud;
using namespace toy;

extern "C"
{
	_BLOCKS_EXPORT void ex_blocks_init(GameShell& app, Game& game);
	_BLOCKS_EXPORT void ex_blocks_start(GameShell& app, Game& game);
	_BLOCKS_EXPORT void ex_blocks_pump(GameShell& app, Game& game);
	_BLOCKS_EXPORT void ex_blocks_scene(GameShell& app, GameScene& scene);
}

_BLOCKS_EXPORT void ex_blocks_pump_impl(GameShell& app, Game& game, Widget& parent, Dockbar* dockbar = nullptr);

enum CustomCollisionGroup : short int
{
	CM_ENERGY_0 = 1 << 10,
	CM_ENERGY_1 = 1 << 11,
	CM_ENERGY_2 = 1 << 12
};

class refl_ _BLOCKS_EXPORT Well : public Complex, public Updatable, public ColliderObject
{
public:
	constr_ Well(Id id, Entity& parent, const vec3& position);
	~Well();

	comp_ attr_ Entity m_entity;
	comp_ attr_ Emitter m_emitter;

	void next_frame(size_t tick, size_t delta);
};

class refl_ _BLOCKS_EXPORT Faction
{
public:
	constr_ Faction(uint32_t id, const Colour& colour, short int energy);

	attr_ uint32_t m_id;
	attr_ Colour m_colour;
	attr_ short int m_energy;

	Material* m_highlight2;
	Material* m_highlight11;

	static const size_t s_max_factions = 10U;
};

class refl_ _BLOCKS_EXPORT Camp : public Complex
{
public:
	constr_ Camp(Id id, Entity& parent, const vec3& position, Faction& faction);

	comp_ attr_ Entity m_entity;

	attr_ vec3 m_position;
	attr_ Faction& m_faction;
};

class refl_ _BLOCKS_EXPORT Shield : public Complex, public Updatable, public ColliderObject
{
public:
	constr_ Shield(Id id, Entity& parent, const vec3& position, Faction& faction, float radius);
	~Shield();

	comp_ attr_ Entity m_entity;
	comp_ attr_ Emitter m_emitter;

	attr_ Faction& m_faction;
	attr_ float m_radius;
	attr_ float m_charge;
	attr_ float m_discharge;

	Solid m_solid;

	void next_frame(size_t tick, size_t delta);
};

class refl_ _BLOCKS_EXPORT Slug : public Complex, public ColliderObject
{
public:
	Slug(Entity& parent, const vec3& source, const quat& rotation, const vec3& velocity, short int energy);
	~Slug();

	comp_ attr_ Entity m_entity;

	attr_ vec3 m_source;
	attr_ vec3 m_velocity;
	attr_ short int m_energy;

	bool m_impacted = false;
	bool m_destroy = false;
	vec3 m_impact = Zero3;

	//Solid m_solid;
	Collider m_collider;

	void update();
};

class refl_ _BLOCKS_EXPORT Tank : public Complex, public Updatable, public ColliderObject
{
public:
	constr_ Tank(Id id, Entity& parent, const vec3& position, Faction& faction);
	~Tank();

	comp_ attr_ Entity m_entity;
	comp_ attr_ Movable m_movable;
	comp_ attr_ Emitter m_emitter;
	comp_ attr_ Receptor m_receptor;
	comp_ attr_ Active m_active;

	Faction& m_faction;

	Solid m_solid;

	vec3 m_force = Zero3;
	vec3 m_torque = Zero3;

	quat m_turret_angle = ZeroQuat;

	float m_cooldown = 0.f;
	float m_hitpoints = 100.f;
	float m_energy = 100.f;
	float m_range = 20.f;

	bool m_stealth = false;
	bool m_control = true;

	Tank* m_target = nullptr;
	vec3 m_dest = Zero3;

	std::vector<object_ptr<Slug>> m_slugs;

	void next_frame(size_t tick, size_t delta);

	void shoot();

	inline quat turret_rotation() { return m_turret_angle; } //quat(vec3(0.f, m_turret_angle, 0.f)); }
	inline vec3 turret_direction() { return rotate(turret_rotation(), -Z3); }
};

class refl_ _BLOCKS_EXPORT BlockWorld : public Complex
{
public:
	constr_ BlockWorld(const std::string& name);
	~BlockWorld();

	comp_ attr_ World m_world;
	comp_ attr_ BulletWorld m_bullet_world;
	comp_ attr_ Navmesh m_navmesh;

	attr_ uvec3 m_block_subdiv = uvec3(20, 4, 20);
	attr_ vec3 m_tile_scale = vec3(4.f);
	attr_ vec3 m_block_size;

	//attr_ uvec3 m_subdiv;
	//attr_ uvec3 m_block_subdiv;
	//attr_ vec3 m_cell_size;

	//attr_ vec3 m_sector_size;
	attr_ vec3 m_world_size;
	//attr_ vec3 m_center_offset;

	//attr_ mut_ std::vector<Element*> m_elements;

	//attr_ mut_ std::vector<Sector*> m_sectors;
	//attr_ mut_ Grid<Block*> m_blocks;

	std::vector<Faction> m_factions;

	std::map<ivec2, TileBlock*> m_blocks;
	TileBlock* m_center_block = nullptr;

	void generate_block(GfxSystem& gfx_system, const ivec2& coord);
};

class refl_ _BLOCKS_EXPORT Player
{
public:
	Player(BlockWorld& world);
	BlockWorld* m_world;
	Tank m_tank;
};

_BLOCKS_EXPORT void paint_tank(Gnode& parent, Tank& tank);
