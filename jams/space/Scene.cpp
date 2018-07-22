
#include <space/Types.h>
#include <space/Scene.h>
#include <space/ex_space.h>

#include <toy/toy.h>

ParticleGenerator jump_particles(VisuFleet& visu_fleet)
{
	UNUSED(visu_fleet);
	ParticleGenerator emitter;
	float scale = 0.02f;
	emitter.m_rate = { 500 };
	emitter.m_colour = { std::vector<Colour>({ Colour::Transparent, Colour::White }) };
	emitter.m_scale = { std::vector<float>({ scale, scale * 2.f }) };
	emitter.m_speed = { 0.f };
	//emitter.m_lifetime = { 0.3f, 0.4f };
	emitter.m_lifetime = { std::vector<float>({ 0.1f, 0.1f }) };
	return emitter;
}

ParticleGenerator in_jump_particles(VisuFleet& visu_fleet)
{
	float scale = 0.02f;
	ParticleGenerator emitter = jump_particles(visu_fleet);
	emitter.m_colour = { Colour::White };
	emitter.m_blend = { std::vector<float>({ 1.f, 0.f }) };
	emitter.m_scale = { std::vector<float>({ scale, scale * 0.3f }) };
	emitter.m_lifetime = { 1.f };
	return emitter;
}

void fill_star(VisuStar& visu, Star& star)
{
	UNUSED(star);
	size_t num_planets = random_integer(3, 12);
	for(size_t i = 0; i < num_planets; ++i)
	{
		float distance = random_scalar(0.f, 1.f);
		float speed = 1.f / (2.f * c_pi * distance) * 10.f;
		float offset = random_scalar(0.f, 2.f * float(c_pi));
		float aniso = random_scalar(0.8f, 1.2f);
		float pitch = random_scalar(0.8f * float(c_pi), 1.2f * float(c_pi));
		float roll = random_scalar(0.8f * float(c_pi), 1.2f * float(c_pi));
		visu.m_planets.push_back({ vec2{ distance, distance * aniso } *0.4f, quat({ pitch, 0.f, roll }), offset, speed });
	}
}

mat4 planet_transform(VisuPlanet& planet, const mat4& transform)
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

void paint_star_ellipses(Gnode& parent, VisuStar& visu)
{
	for(size_t i = 0; i < visu.m_planets.size(); ++i)
		paint_planet_ellipsis(parent, visu.m_planets[i]);
}

void paint_star_planets(Gnode& parent, Star& star)
{
	VisuStar& visu = star.m_visu;

	if(visu.m_updated == 0)
	{
		fill_star(visu, star);
		visu.m_updated = 1;
	}

	Item& item = gfx::shape(parent, Sphere(0.005f), Symbol(Colour::None, Colour::White), 0, nullptr, visu.m_planets.size());
	mat4 transform = parent.m_attach->transform();

	for(size_t i = 0; i < visu.m_planets.size(); ++i)
	{
		item.m_instances[i] = planet_transform(visu.m_planets[i], transform);
	}

	//paint_star_ellipses(parent, star);
}

void paint_star(Gnode& parent, Star& star)
{
	paint_star_planets(parent, star);
}

void paint_range_grid(Gnode& parent, const Colour& colour, int range)
{
	float side = float(range * 2 + 1);
	Gnode& projected = gfx::node(parent, {}, { parent.m_attach->m_position.x, 0.5f, parent.m_attach->m_position.z });
	gfx::shape(projected, Grid2(vec2(side), vec2(2.5f)), Symbol(colour));
}

void paint_range(Gnode& parent, const Colour& colour, int range)
{
	float side = float(range * 2 + 1);
	Gnode& projected = gfx::node(parent, {}, { parent.m_attach->m_position.x, 0.5f, parent.m_attach->m_position.z });
	gfx::shape(projected, Quad(side, X3, Z3), Symbol(colour));
}

inline float sec(float a) { return 1.f / cos(a); }
inline float csc(float a) { return 1.f / sin(a); }

void paint_range_sonar(Gnode& parent, const Colour& colour, int range)
{
	static Clock clock;

	float angle = fmod(float(clock.read()), 2.f * c_pi);
	float length = min(abs(sec(angle)), abs(csc(angle))) * range;
	vec3 end = rotate(X3 * length, angle, Y3);

	Gnode& projected = gfx::node(parent, {}, { parent.m_attach->m_position.x, 0.5f, parent.m_attach->m_position.z });
	gfx::draw(projected, Line(Zero3, end), Symbol(colour));
}

void paint_scan_star(Gnode& parent, Star& star, Player& player)
{
	gfx::shape(parent, Circle(0.4f, Axis::Y), Symbol(Colour::None, Colour::Invisible), ITEM_SELECTABLE);
	gfx::shape(parent, Sphere(0.01f), Symbol(Colour::None, Colour::White * 4.f), ITEM_SELECTABLE);

	bool hovered = player.m_hovered_item == Ref(&star);
	bool selected = player.m_selected_item == Ref(&star);

	Colour colour = star.m_commander ? star.m_commander->m_colour * 3.f : Colour{ 0.3f, 0.3f, 0.3f, 0.1f };
	if(player.m_mode == GameMode::Empire && star.m_commander != player.m_commander)
		colour = saturation(colour, 0.7f);
	if(hovered)
		colour = colour * 1.5f;

	if(star.m_commander)
		gfx::sprite(parent, star.m_commander->m_avatar, vec2{ 0.4f }, ITEM_BILLBOARD | ITEM_SELECTABLE);

	gfx::shape(parent, Circle(0.4f, Axis::Y), Symbol(colour));
	//gfx::shape(parent, Circle(0.4f, Axis::Z), Symbol(colour), ITEM_BILLBOARD);

	if(star.m_commander == player.m_commander)
	{
		if(star.m_commander->m_capital == &star)
			gfx::shape(parent, Circle(0.5f, Axis::Y), Symbol(colour));

		if(!star.m_constructions.empty())
			gfx::shape(parent, Quad(0.5f, X3, Z3), Symbol(colour));

		if(star.m_revolt)
			gfx::shape(parent, Circle(0.3f, Axis::Y), Symbol(Colour::Red));
	}

	if((selected || hovered) && star.m_commander == player.m_commander)
	{
		if(star.m_scan > 0)
			paint_range(parent, colour, star.m_scan);
	}
}

void paint_scan_star(Gnode& parent, Star& star)
{
	Player& player = val<Player>(parent.root().m_scene->m_user);
	paint_scan_star(parent, star, player);
}

static float spaceship_sizes[8] = { 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.1f };

void fill_fleet(VisuFleet& visu, const std::map<ShipSchema*, size_t>& ships)
{
	for(size_t i = 0; i < 8; ++i)
		visu.m_ships[i].clear();

	visu.m_points.m_points.clear();

	for(auto& kv : ships)
	{
		ShipSchema& schema = *kv.first;
		for(size_t i = 0; i < kv.second; ++i)
		{
			visu.m_ships[schema.m_size].push_back({ spaceship_sizes[schema.m_size] });
			visu.m_ships[schema.m_size].back().m_weapon = schema.m_main_weapon;
			visu.m_ships[schema.m_size].back().m_destroyed = false;
		}
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
			ship.m_cooldown = random_scalar(0.f, 1.f);
		}
	}
}

vec3 ship_position(VisuFleet& fleet, VisuShip& ship, float radius)
{
	quat rot = angle_axis(fleet.m_angle, Z3);
	return rotate(rot, fleet.m_dilate * ship.m_spot * radius);
}

void update_fleet_positions(VisuFleet& visu, float radius)
{
	size_t index = 0;
	for(size_t size = 0; size < 8; ++size)
		for(size_t i = 0; i < visu.m_ships[size].size(); ++i)
		{
			visu.m_ships[size][i].m_position = ship_position(visu, visu.m_ships[size][i], radius);
			visu.m_points.m_points[index++] = visu.m_ships[size][i].m_position;
		}
}

mat4 ship_transform(VisuShip& ship, const mat4& transform, const vec3& position, float ship_scale)
{
	vec3 scale = ship.m_destroyed ? Zero3 : vec3{ ship.m_radius  * ship_scale };
	mat4 ship_mat = bxSRT(scale, Zero3, position);
	return bxmul(ship_mat, transform);
}

void paint_fleet_ships(Gnode& parent, VisuFleet& visu, float radius, float ship_scale)
{
	//toy::sound(parent, "thrusters", true);

	update_fleet_positions(visu, radius);

	size_t index = 0;
	for(size_t size = 0; size < 8; ++size)
		if(!visu.m_ships[size].empty())
		{
			Item* item = gfx::model(parent, "spaceship", ITEM_LOD_0, nullptr, visu.m_ships[size].size());
			mat4 transform = parent.m_attach->transform();
			for(size_t i = 0; i < visu.m_ships[size].size(); ++i)
				item->m_instances[i] = ship_transform(visu.m_ships[size][i], transform, visu.m_points.m_points[index++], ship_scale);
		}
}

void debug_tree(Gnode& node, size_t index = 0, size_t depth = 0)
{
	auto print_depth = [](size_t depth) { for(size_t i = 0; i < depth; ++i) printf("    "); };
	print_depth(depth);
	printf("node %i\n", int(index));
	if(node.m_item)
	{
		print_depth(depth + 1);
		printf("item %s\n", node.m_item->m_model->m_name.c_str());
	}
	for(size_t i = 0; i < node.m_nodes.size(); ++i)
		debug_tree(*node.m_nodes[i], i, depth + 1);
}

void paint_fleet_orders(Gnode& parent, Fleet& fleet, const Colour& colour)
{
	if(fleet.m_jump.m_state == FleetJump::Ordered)
	{
		vec3 start = to_xz(vec2(fleet.m_coord)) + 0.5f;
		vec3 end = to_xz(vec2(fleet.m_jump.m_destination)) + 0.5f;
		vec3 middle = (end + start) / 2.f + Y3 * 2.f;// * length(end - start);
		gfx::shape(parent, ArcLine(-start, start, middle, end), Symbol(colour));
	}
	//else
	//{
	//	debug_tree(*parent.m_parent);
	//}
}

void paint_fleet_jumping(Gnode& parent, VisuFleet& visu, const Colour& colour)
{
	static ParticleGenerator particles = jump_particles(visu);
	//particles.m_shape = &visu.m_points;
	gfx::particles(parent, particles);
	gfx::shape(parent, Circle(0.4f, Axis::Z), Symbol(colour, Colour::Invisible), ITEM_BILLBOARD);
}

void paint_fleet_warp(Gnode& parent, VisuFleet& visu)
{
	static ParticleGenerator particles = in_jump_particles(visu);
	//particles.m_shape = &visu.m_points;
	gfx::particles(parent, particles);
}

void paint_fleet_stationary(Gnode& parent, VisuFleet& visu, const Colour& colour)
{
	gfx::shape(parent, Triangle({ 0.4f, 0.6f }), Symbol(colour, Colour::Invisible), ITEM_BILLBOARD | ITEM_SELECTABLE);
	paint_fleet_ships(parent, visu);
}

void paint_scan_fleet(Gnode& parent, Fleet& fleet, Player& player)
{
	enum States {  Fleet = 0, Warp = 1, Jump = 2, Orders = 3, ScanRange = 4, JumpRange = 5 };

	VisuFleet& visu = fleet.m_visu;

	if(fleet.m_ships_updated > visu.m_updated)
	{
		fill_fleet(visu, fleet.m_ships);
		visu.m_updated = fleet.m_ships_updated;
	}

	bool hovered = player.m_hovered_item == Ref(&fleet);
	bool selected = player.m_selected_item == Ref(&fleet);

	Colour colour = fleet.m_commander->m_colour * 2.5f;
	if(player.m_mode == GameMode::Empire && fleet.m_commander != player.m_commander)
		colour = saturation(colour, 0.7f);
	if(hovered)
		colour = colour * 1.5f;

	//if(fleet.m_jump.m_state != FleetJump::Jump)
		paint_fleet_stationary(parent.subx(Fleet), visu, colour);
	if(fleet.m_jump.m_state >= FleetJump::Start)
		toy::sound(parent.subx(Warp), "warp", false);
	if(fleet.m_jump.m_state == FleetJump::Start || fleet.m_jump.m_state == FleetJump::End)
		paint_fleet_jumping(parent.subx(Jump), visu, colour);
	//if(fleet.m_jump.m_state == FleetJump::Jump || fleet.m_jump.m_state == FleetJump::End)
	//	paint_fleet_warp(parent, visu);

	if(fleet.m_commander == player.m_commander)
		paint_fleet_orders(parent.subx(Orders), fleet, colour);

	if(player.m_mode == GameMode::Empire && fleet.m_commander == player.m_commander)
	{
		paint_range(parent.subx(ScanRange), colour, fleet.m_scan);
	}

	if((selected || hovered) && fleet.m_commander == player.m_commander)
	{
		paint_range(parent.subx(JumpRange), colour, fleet.m_speed);
		//if(fleet.m_scan > 0)
			//paint_range(parent, fleet.m_commander->m_colour * 2.f, fleet.m_scan);
	}
}

void paint_scan_fleet(Gnode& parent, Fleet& fleet)
{
	Player& player = val<Player>(parent.root().m_scene->m_user);
	paint_scan_fleet(parent, fleet, player);
}

static float s_weapon_rate[6]  = { 0.f, 2.f,   1.2f,  1.f,    0.7f,  1.f };
static float s_weapon_speed[6] = { 0.f, 2.f,   1.2f,  1.f,    0.7f,  1.f };
static float s_weapon_scale[6] = { 0.f, 0.01f, 0.01f, 0.03f,  0.05f, 0.05f };

static float s_weapon_hue[6]   = { 0.f, 0.55f, 1.f,   0.305f, 0.f,   0.f };
static float s_weapon_sat[6]   = { 0.f, 1.f,   1.f,   1.f,    0.f,   0.f };
static float s_weapon_value[6] = { 0.f, 0.7f,  0.6f,  0.6f,   0.8f,  0.8f };

static Colour s_weapon_colour[6] =
{
	Colour(),
	hsl_to_rgb(0.55f, 1.f, 0.7f),
	hsl_to_rgb(1.f, 1.f, 0.6f),
	hsl_to_rgb(0.305f, 1.f, 0.6f),
	hsl_to_rgb(0.f, 0.f, 0.8f),
	hsl_to_rgb(0.f, 0.f, 0.8f)
};

static Cube s_weapon_shape[6] =
{
	Cube(),
	Cube(vec3(vec2(0.001f), 0.004f)),
	Cube(vec3(vec2(0.001f), 0.004f)),
	Cube(vec3(vec2(0.001f), 0.004f)),
	Cube(vec3(vec2(0.001f), 0.004f)),
	Cube(vec3(vec2(0.001f), 0.004f))
};

void scale_down(ParticleGenerator& particles, float factor)
{
	particles.m_scale.m_value *= factor;
	particles.m_volume.m_value *= factor;
	particles.m_speed.m_value *= factor;
}

unique_ptr<ParticleGenerator> weapon_particles(Gnode& parent, cstring name, WeaponType weapon)
{
	ParticleGenerator& original = *parent.m_scene->m_gfx_system.particles().file(name);
	unique_ptr<ParticleGenerator> particles = make_unique<ParticleGenerator>(original);
	particles->m_colour.m_value = s_weapon_colour[size_t(weapon)];
	//particles->m_scale.m_value = s_weapon_scale[size_t(weapon)];
	scale_down(*particles, 0.005f);
	return std::move(particles);
}

bool paint_weapon_ray(Gnode& parent, const WeaponRay& ray, WeaponType weapon)
{
	static unique_ptr<ParticleGenerator> weapon_flash[4] = {};
	static unique_ptr<ParticleGenerator> weapon_trail[4] = {};
	static unique_ptr<ParticleGenerator> weapon_impact[4] = {};

	if(weapon_flash[size_t(weapon)] == nullptr)
	{
		weapon_flash[size_t(weapon)] = weapon_particles(parent, "flash", weapon);
		weapon_trail[size_t(weapon)] = weapon_particles(parent, "impact", weapon);
		weapon_impact[size_t(weapon)] = weapon_particles(parent, "impact", weapon);
	}

	Colour colour = s_weapon_colour[size_t(weapon)] * 3.f;

	vec3 current = ray.m_start + ray.m_dir * ray.m_d;

	//Gnode& source = gfx::node(parent, {}, ray.m_start, ray.m_angle);
	Gnode& projectile = gfx::node(parent, {}, current, ray.m_angle);
	//Gnode& hit = gfx::node(parent, {}, ray.m_end, ray.m_angle);

	//gfx::particles(out, flash);

	if(ray.m_d < ray.m_length)
	{
		gfx::shape(projectile, s_weapon_shape[size_t(weapon)], Symbol(Colour::None, colour));
		//gfx::particles(projectile, trail);
		return false;
	}
	else
	{
		//return gfx::particles(hit, *weapon_impact[size_t(weapon)]).m_ended;
		return true;
	}
}

namespace mud
{
	template <> Type& type<VisuShip>() { static Type ty; return ty; }
}

template <class T>
T random_element(const std::vector<T>& vec)
{
	return vec[random_integer<size_t>(0, vec.size() - 1)];
}

void paint_combat_fleet(Gnode& parent, const std::vector<CombatFleet>& flotilla, const std::vector<CombatFleet>& enemies, float delta, float intensity)
{
	for(const CombatFleet& combat_fleet : flotilla)
	{
		Fleet* fleet = combat_fleet.m_fleet;
		Fleet* target_fleet = random_element(enemies).m_fleet;

		for(uint8_t i = 0; i < 8; ++i)
			for(VisuShip& ship : fleet->m_visu.m_ships[i])
			{
				Gnode& node = gfx::node(parent, Ref(&ship), fleet->m_entity.m_position + ship.m_position);

				if(ship.m_destroyed)
				{
					static ParticleGenerator& explode = *parent.m_scene->m_gfx_system.particles().file("explode");
					static bool once = false;
					if(!once)
					{
						scale_down(explode, 0.1f);
						once = true;
					}
					gfx::particles(node, explode);
				}

				if(ship.m_destroyed || ship.m_weapon == WeaponType::None)
					continue;

				ship.m_cooldown -= intensity * 100.f;
				if(ship.m_cooldown < 0.f)
				{
					if(ship.m_ray.m_end == Zero3)
					{
						vec3 source = fleet->m_entity.m_position + ship.m_position;
						vec3 target = target_fleet->m_entity.m_position + random_element(target_fleet->m_visu.m_points.m_points);
						quat angle = look_at(source, target);
						ship.m_ray = { source, target, normalize(target - source), length(target - source), angle, 0.f };
					}

					ship.m_ray.m_d += delta * s_weapon_speed[size_t(ship.m_weapon)];

					if(paint_weapon_ray(node, ship.m_ray, ship.m_weapon))
					{
						ship.m_ray = {};
						ship.m_cooldown = 2.f / s_weapon_rate[size_t(ship.m_weapon)];
					}
				}
			}
	}
}

void paint_combat(Gnode& parent, SpatialCombat& combat)
{
	toy::sound(parent, "hornwar");

	vec3 offset = Z3 * 0.5f;

	if(combat.m_state == SpatialCombat::APPROACH)
	{
		for(CombatFleet& combat_fleet : combat.m_allies)
			combat_fleet.m_fleet->m_entity.m_position = combat_fleet.m_fleet->base_position() - offset;
		for(CombatFleet& combat_fleet : combat.m_enemies)
			combat_fleet.m_fleet->m_entity.m_position = combat_fleet.m_fleet->base_position() + offset;

		combat.m_state = SpatialCombat::ENGAGE;
	}

	static Clock clock;
	float delta = float(clock.step());

	const float length = 20.f;

	float dt = delta / length;

	combat.m_t += dt;
	combat.m_t = min(1.f, combat.m_t);

	float s = sin(combat.m_t * c_pi);
	combat.m_t_position = s;

	combat.m_dt_intensity = s * 2.f * dt;
	combat.m_t_damage += combat.m_dt_intensity;
	combat.m_t_damage = min(1.f, combat.m_t_damage);

	auto destroy_ships = [&](Fleet& fleet, uint32_t* hull_losses)
	{
		for(uint8_t t = 0; t < 8; ++t)
		{
			size_t last = uint32_t(float(hull_losses[t]) * combat.m_t_damage);
			for(size_t i = 0; i < last; ++i)
				fleet.m_visu.m_ships[t][i].m_destroyed = true;
		}
	};

	for(CombatFleet& combat_fleet : combat.m_allies)
		destroy_ships(*combat_fleet.m_fleet, combat_fleet.m_hull_losses.data());
	for(CombatFleet& combat_fleet : combat.m_enemies)
		destroy_ships(*combat_fleet.m_fleet, combat_fleet.m_hull_losses.data());

	for(CombatFleet& combat_fleet : combat.m_allies)
		combat_fleet.m_fleet->m_entity.m_position = combat_fleet.m_fleet->base_position() - offset + offset * combat.m_t_position;
	for(CombatFleet& combat_fleet : combat.m_enemies)
		combat_fleet.m_fleet->m_entity.m_position = combat_fleet.m_fleet->base_position() + offset - offset * combat.m_t_position;

	paint_combat_fleet(parent, combat.m_allies, combat.m_enemies, delta, combat.m_dt_intensity);
	paint_combat_fleet(parent, combat.m_enemies, combat.m_allies, delta, combat.m_dt_intensity);
}

void galaxy_grid(Gnode& parent, Galaxy& galaxy)
{
	Colour colour = Colour::White * 3.f;//{ 0.3f, 0.3f, 0.3f, 0.04f };
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
	if(galaxy.m_highlight != uvec2(UINT32_MAX))
		highlighted_sector(parent, galaxy.m_highlight);
}

void paint_scene(Gnode& parent)
{
#ifdef TOY_SOUND
	parent.m_sound_manager->threadUpdate();
#endif

	Light& light = gfx::directional_light_node(parent);
	light.m_shadows = false;

	gfx::radiance(parent, "equirectangular_space_sampler.jpg", BackgroundMode::None);
	//gfx::custom_sky(parent, "start_nest");

	toy::sound(parent, "deep", true);

	static float crackle = 170.f;
	static float grtgrtttt = 55.f;
	static float grzzt = 0.f;

	static Clock clock;
	float step = float(clock.step());

	auto update_sound = [&](float& counter, cstring name, float period, float volume)
	{
		counter += step;
		if(counter > period)
		{
			toy::sound(parent, name, false, volume);
			counter = 0.f;
		}
	};

	update_sound(crackle, "crackle", 180.f, 0.3f);
	//update_sound(grtgrtttt, "grtgrtttt", 60.f, 0.1f);
	//update_sound(grzzt, "grzzt", 10.f);
}

void paint_viewer(Viewer& viewer)
{
	viewer.m_filters.m_glow.m_enabled = true;
	viewer.m_filters.m_glow.m_levels_1_4 = { 1.f, 1.f, 0.f, 0.f };
	viewer.m_filters.m_glow.m_bicubic_filter = true;
}