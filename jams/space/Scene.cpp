
#include <space/Scene.h>
#include <space/ex_space.h>

#include <toy/toy.h>

struct VisuPlanet
{
	VisuPlanet(vec2 ellipsis, quat rotation, float offset, float speed) : m_ellipsis(ellipsis), m_rotation(rotation), m_speed(speed), m_period(offset) {}
	vec2 m_ellipsis;
	quat m_rotation;
	float m_speed;
	float m_period = 0.f;
};

struct VisuStar : public NodeState
{
	Commander* m_commander = nullptr;
	std::vector<VisuPlanet> m_planets;
	float m_period = 0.f;
	size_t m_updated = 0;
	bool m_once = false;
};

void fill_star(VisuStar& visu, Star& star)
{
	size_t num_planets = random_integer(3, 12);
	for(size_t i = 0; i < num_planets; ++i)
	{
		float distance = random_scalar(0.f, 1.f);
		float speed = 1.f / (2.f * M_PI * distance) * 10.f;
		float offset = random_scalar(0.f, 2.f * float(M_PI));
		float aniso = random_scalar(0.8f, 1.2f);
		float pitch = random_scalar(0.8f * float(M_PI), 1.2f * float(M_PI));
		float roll = random_scalar(0.8f * float(M_PI), 1.2f * float(M_PI));
		visu.m_planets.push_back({ vec2{ distance, distance * aniso } *0.4f, quat({ pitch, 0.f, roll }), offset, speed });
	}
}

mat4 planet_transform(VisuStar& star, VisuPlanet& planet, const mat4& transform)
{
	vec2 coord = vec2{ cos(planet.m_period), sin(planet.m_period) } * planet.m_ellipsis;
	mat4 planet_mat = bxSRT(Unit3, Zero3, rotate(planet.m_rotation, vec3{ coord.x, 0.f, coord.y }));
	return bxmul(planet_mat, transform);
}

void paint_planet_ellipsis(Gnode& parent, VisuPlanet& planet)
{
	gfx::node(parent, {}, rotate(planet.m_rotation, parent.m_attach->m_position), planet.m_rotation);
	gfx::shape(parent, Ellipsis(planet.m_ellipsis), Symbol(Colour::AlphaGrey, Colour::None), ITEM_SELECTABLE);
}

void paint_star_ellipses(Gnode& parent, Star& star)
{
	VisuStar& visu = parent.state<VisuStar>();

	for(size_t i = 0; i < visu.m_planets.size(); ++i)
		paint_planet_ellipsis(parent, visu.m_planets[i]);
}

void paint_star_planets(Gnode& parent, Star& star)
{
	VisuStar& visu = parent.state<VisuStar>();

	//if(fleet.m_last_updated > visu.m_updated)
	if(!visu.m_once)
	{
		visu.m_once = true;
		fill_star(visu, star);
	}

	float speed = 0.001f;
	size_t ticks = star.m_entity.m_lastTick - visu.m_updated;
	visu.m_updated = star.m_entity.m_lastTick;
	visu.m_period = fmod(visu.m_period + ticks * speed, 2 * M_PI);

	Item& item = gfx::shape(parent, Sphere(0.005f), Symbol(Colour::None, Colour::White), 0, nullptr, visu.m_planets.size());
	mat4 transform = parent.m_attach->transform();

	for(size_t i = 0; i < visu.m_planets.size(); ++i)
	{
		visu.m_planets[i].m_period = fmod(visu.m_planets[i].m_period + ticks * speed * visu.m_planets[i].m_speed, 2 * M_PI);
		item.m_instances[i] = planet_transform(visu, visu.m_planets[i], transform);
	}

	//paint_star_ellipses(parent, star);
}

void paint_star(Gnode& parent, Star& star)
{
	VisuStar& visu = parent.state<VisuStar>();

	if(visu.m_commander != star.m_commander)
	{
		// @todo this is required cause some state of the nodes doesn't get updated
		// shape color is not immediate because it is stored in the symbol mesh
		parent.clear();
		visu.m_commander = star.m_commander;
	}

	Gnode& commander = parent;
	if(star.m_commander)
		gfx::sprite(commander, star.m_commander->m_avatar, vec2{ 0.4f }, ITEM_BILLBOARD | ITEM_SELECTABLE);

	Colour colour = star.m_commander ? star.m_commander->m_colour : Colour::AlphaGrey;
	//gfx::shape(parent, Symbol(Colour::AlphaGrey, Colour::Invisible), Circle(0.4f, Axis::Z), ITEM_BILLBOARD | ITEM_SELECTABLE);
	gfx::shape(parent, Circle(0.4f, Axis::Y), Symbol(colour, Colour::Invisible), ITEM_SELECTABLE);
	gfx::shape(parent, Sphere(0.01f), Symbol(Colour::None, Colour::White), ITEM_SELECTABLE);
	paint_star_planets(parent, star);
}

static float spaceship_sizes[8] = { 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.1f };

struct VisuShip
{
	float m_radius;
	vec3 m_spot;
	float m_depth;
};

struct VisuFleet : public NodeState
{
	std::vector<VisuShip> m_ships[8];
	Points m_points;
	float m_angle = 0.f;
	float m_offset = 0.f;
	vec3 m_dilate = Unit3;
	size_t m_updated = 0;
	bool m_once = false;
};

vec3 ship_position(VisuFleet& fleet, VisuShip& ship)
{
	float radius = 0.4f;
	quat rot = angleAxis(fleet.m_angle, vec3{ 0.f, 0.f, 1.f });
	return rotate(rot, fleet.m_dilate * ship.m_spot * radius);
}

mat4 ship_transform(VisuFleet& fleet, VisuShip& ship, const mat4& transform, const vec3& position)
{
	mat4 ship_mat = bxSRT(vec3{ ship.m_radius }, Zero3, position);
	return bxmul(ship_mat, transform);
}

void fill_fleet(VisuFleet& visu, Fleet& fleet)
{
	for(auto& kv : fleet.m_composition)
	{
		ShipSchema& schema = *kv.first;
		for(size_t i = 0; i < kv.second; ++i)
			visu.m_ships[schema.m_size].push_back({ spaceship_sizes[schema.m_size] });
	}

	Poisson distribution = { vec2{ 1.f }, spaceship_sizes[7] };
	distribution.m_start_from_center = true;

	for(size_t size = 0; size < 8; ++size)
	{
		visu.m_points.m_points.resize(visu.m_points.m_points.size() + visu.m_ships[size].size());

		for(VisuShip& ship : visu.m_ships[size])
		{
			distribution.addPoint(ship.m_radius, ship.m_spot);
			ship.m_depth = random_scalar(0.f, 1.f);
			ship.m_spot.z = ship.m_depth;
			ship.m_spot = ship.m_spot * 2.f - 1.f;
		}
	}
}

void update_fleet_positions(VisuFleet& visu, Fleet& fleet)
{
	size_t index = 0;
	for(size_t size = 0; size < 8; ++size)
		for(size_t i = 0; i < visu.m_ships[size].size(); ++i)
			visu.m_points.m_points[index++] = ship_position(visu, visu.m_ships[size][i]);
}

void paint_fleet_ships(Gnode& parent, Fleet& fleet, VisuFleet& visu)
{
	float speed = 0.001f;
	size_t ticks = fleet.m_entity.m_lastTick - visu.m_updated;
	visu.m_updated = fleet.m_entity.m_lastTick;
	visu.m_angle = fmod(visu.m_angle + ticks * speed, 2 * M_PI);
	visu.m_offset = sin(visu.m_updated * speed);
	visu.m_dilate.z = remap_trig(visu.m_offset, 0.7f, 1.f);

	size_t index = 0;
	for(size_t size = 0; size < 8; ++size)
		if(!visu.m_ships[size].empty())
		{
			Item& item = gfx::model(parent, "spaceship", ITEM_LOD_0, nullptr, visu.m_ships[size].size());
			mat4 transform = parent.m_attach->transform();
			for(size_t i = 0; i < visu.m_ships[size].size(); ++i)
				item.m_instances[i] = ship_transform(visu, visu.m_ships[size][i], transform, visu.m_points.m_points[index++]);
		}
}

ParticleGenerator jump_particles(Gnode& parent, VisuFleet& visu_fleet)
{
	ParticleSystem& particle_system = parent.m_scene->m_gfx_system.m_pipeline.block<BlockParticles>()->m_particle_system;

	ParticleGenerator emitter;
	float radius = 0.4f;
	float scale = 0.02f;
	emitter.m_rate = { 500 };
	emitter.m_colour = { std::vector<Colour>({ Colour::Transparent, Colour::White }) };
	emitter.m_scale = { std::vector<float>({ scale, scale * 2.f }) };
	emitter.m_speed = { 0.f };
	//emitter.m_lifetime = { 0.3f, 0.4f };
	emitter.m_lifetime = { std::vector<float>({ 0.1f, 0.1f }) };
	return emitter;
}

ParticleGenerator in_jump_particles(Gnode& parent, VisuFleet& visu_fleet)
{
	float scale = 0.02f;
	ParticleGenerator emitter = jump_particles(parent, visu_fleet);
	emitter.m_colour = { Colour::White };
	emitter.m_blend = { std::vector<float>({ 1.f, 0.f }) };
	emitter.m_scale = { std::vector<float>({ scale, scale * 0.3f }) };
	emitter.m_lifetime = { 1.f };
	return emitter;
}

void paint_fleet_jumping(Gnode& parent, Fleet& fleet, VisuFleet& visu, const Colour& colour)
{
	if(!(fleet.m_jump.m_state == FleetJump::START || fleet.m_jump.m_state == FleetJump::END))
		return;
	static ParticleGenerator particles = jump_particles(parent, visu);
	//particles.m_shape = &visu.m_points;
	gfx::particles(parent, particles);
	gfx::shape(parent, Circle(0.4f, Axis::Z), Symbol(colour, Colour::Invisible), ITEM_BILLBOARD);
}

void paint_fleet_warp(Gnode& parent, Fleet& fleet, VisuFleet& visu, const Colour& colour)
{
	return;

	if(!(fleet.m_jump.m_state == FleetJump::JUMP || fleet.m_jump.m_state == FleetJump::END))
		return;
	static ParticleGenerator particles = in_jump_particles(parent, visu);
	//particles.m_shape = &visu.m_points;
	gfx::particles(parent, particles);
	snd_sound(parent, "Jump", false);
}

void paint_fleet_stationary(Gnode& parent, Fleet& fleet, VisuFleet& visu, const Colour& colour)
{
	if(fleet.m_jump.m_state == FleetJump::JUMP)
		return;
	gfx::shape(parent, Triangle({ 0.4f, 0.6f }), Symbol(colour, Colour::Invisible), ITEM_BILLBOARD | ITEM_SELECTABLE);
	paint_fleet_ships(parent, fleet, visu);
}

float s_weapon_rate[4] = { 2.f, 1.2f, 1.f, 0.7f };
float s_weapon_scale[4] = { 0.01f, 0.01f, 0.03f, 0.05f };
Colour s_weapon_colour[4] = { Colour::Cyan, Colour(1.f, 0.4f, 0.6f), Colour::Pink, Colour::White };

ParticleGenerator battle_particles(Gnode& parent)
{
	ParticleSystem& particle_system = parent.m_scene->m_gfx_system.m_pipeline.block<BlockParticles>()->m_particle_system;

	ParticleGenerator emitter;
	float radius = 0.4f;
	float scale = 0.02f;
	emitter.m_rate = { 50 };
	emitter.m_colour = { Colour::Pink };
	emitter.m_scale = { scale };
	emitter.m_speed = { 2.f };
	emitter.m_lifetime = { 1.f };
	emitter.m_direction = EmitterDirection::Absolute;
	return emitter;
}

void paint_combat_rays(Gnode& parent, Fleet& fleet, VisuFleet& visu, WeaponType weapon_type)
{
	static ParticleGenerator particles = battle_particles(parent);
	float rate = fleet.m_weapon_ratio[size_t(weapon_type)] * s_weapon_rate[size_t(weapon_type)] * 50.f;
	particles.m_rate = { uint32_t(rate), uint32_t(rate) };
	particles.m_colour = { s_weapon_colour[size_t(weapon_type)], s_weapon_colour[size_t(weapon_type)] };
	particles.m_scale = { s_weapon_scale[size_t(weapon_type)], s_weapon_scale[size_t(weapon_type)] };
	//particles.m_shape = &visu.m_points;
	particles.m_absolute_direction = fleet.m_combat_direction;
	gfx::particles(parent, particles);
}

void paint_fleet_combat(Gnode& parent, Fleet& fleet, VisuFleet& visu)
{
	static ParticleGenerator particles = jump_particles(parent, visu);
	//particles.m_shape = &visu.m_points;
	gfx::particles(parent, particles);

	for(WeaponType weapon_type : s_weapon_types)
		paint_combat_rays(parent, fleet, visu, weapon_type);
}

void paint_fleet(Gnode& parent, Fleet& fleet)
{
	VisuFleet& visu = parent.state<VisuFleet>();

	//if(fleet.m_last_updated > visu.m_updated)
	if(!visu.m_once)
	{
		visu.m_once = true;
		fill_fleet(visu, fleet);
	}

	update_fleet_positions(visu, fleet);

	Colour colour = fleet.m_commander ? fleet.m_commander->m_colour : Colour::AlphaGrey;

	paint_fleet_stationary(parent, fleet, visu, colour);
	paint_fleet_jumping(parent, fleet, visu, colour);
	paint_fleet_warp(parent, fleet, visu, colour);

	if(fleet.m_combat)
		paint_fleet_combat(parent, fleet, visu);
}

#if 0
void paint_combat(Gnode& parent, SpatialCombat& combat)
{
	for(Fleet* fleet : combat.m_flotilla_0)
		for(uint8_t i = 0; i < 8; ++i)
			paint_combat_rays(parent, *fleet, i);

	for(Fleet* fleet : combat.m_flotilla_1)
		for(uint8_t i = 0; i < 8; ++i)
			paint_combat_rays(parent, *fleet, i);

}
#endif

void galaxy_grid(Gnode& parent, Galaxy& galaxy)
{
	Colour colour = { 0.3f, 0.3f, 0.3f, 0.04f };
	Gnode& self = gfx::node(parent, {}, vec3{ 0.f, 0.5f, 0.f });
	gfx::shape(self, Grid2(galaxy.m_size), Symbol(colour));
}

void highlighted_sector(Gnode& parent, const vec2& coord)
{
	Gnode& self = gfx::node(parent, {}, vec3{ coord.x, 0.f, coord.y } + 0.5f);
	gfx::shape(self, Rect(0.f, 0.f, 1.f, 1.f), Symbol(Colour::White)); // Quad(1.f, X3, Z3)
}

void paint_galaxy(Gnode& parent, Galaxy& galaxy)
{
	Light& light = gfx::directional_light_node(parent);
	light.m_shadows = false;

	gfx::radiance(parent, "equirectangular_space_sampler.jpg", BackgroundMode::None);
	//gfx::custom_sky(parent, "start_nest");

	galaxy_grid(parent, galaxy);

	snd_sound(parent, "Wind", true);

	if(galaxy.m_highlight != uvec2(UINT32_MAX))
		highlighted_sector(parent, galaxy.m_highlight);
}