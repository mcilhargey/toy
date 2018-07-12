
#include <platform/ex_platform.h>
#include <toy/toy.h>

#include <platform/Api.h>
#include <meta/platform/Module.h>

//#define _PLATFORM_TOOLS

void populate_block(TileBlock& block)
{
	generate_crates(block);
	generate_npcs(block);

	for(size_t x = 0; x < block.m_wfc_block.m_tiles.m_x; ++x)
		for(size_t y = 0; y < block.m_wfc_block.m_tiles.m_y; ++y)
			for(size_t z = 0; z < block.m_wfc_block.m_tiles.m_z; ++z)
			{
				Tile& tile = block.m_wfc_block.m_tileset.m_tiles_flip[block.m_wfc_block.m_tiles.at(x, y, z)];
				if(tile.m_name == "cube_covered_side")
				{
					if(random_integer(0, 9) > 8)
						block.m_entity.m_world.origin().construct<Lamp>(block.m_wfc_block.to_position(uvec3(x, y, z)) + Y3 * 1.5f * block.m_wfc_block.m_scale);
				}
			}
}

TileWorld::TileWorld(const std::string& name)
	: Complex(0, type<TileWorld>(), m_bullet_world, m_navmesh, *this)
	, m_world(0, *this, name)
	, m_bullet_world(m_world)
	, m_navmesh(m_world)
	, m_block_size(vec3(m_block_subdiv) * m_tile_scale)
{}

TileWorld::~TileWorld()
{
	m_world.destroy();
}

void TileWorld::next_frame()
{
	for(auto& coord_block : m_blocks)
		if(coord_block.second && coord_block.second->m_setup && !coord_block.second->m_populated)
		{
			populate_block(*coord_block.second);
			coord_block.second->m_populated = true;
		}
}

void TileWorld::generate_block(GfxSystem& gfx_system, const ivec2& coord)
{
	static WaveTileset& tileset = generator_tileset(gfx_system);

	TileBlock& block = ::generate_block(gfx_system, tileset, m_world.origin(), coord, m_block_subdiv, m_tile_scale);

	block.m_world_page.m_geometry_filter = { "platform/cube_covered", "platform/cube_covered_side", "platform/cube_covered_angle", "platform/corner_covered", "platform/empty_covered" };

	m_blocks[coord] = &block;

	for(size_t x : { 0U, block.m_wfc_block.m_tiles.m_x - 1 })
	for(size_t y = 0; y < block.m_wfc_block.m_tiles.m_y; ++y)
	for(size_t z = 0; z < block.m_wfc_block.m_tiles.m_z; ++z)
	{
		uint16_t empty = 0;
		block.m_wfc_block.m_wave.set_tile({ uint(x), uint(y), uint(z) }, empty);
	}

	if(m_center_block == nullptr)
	{
		m_center_block = &block;
		block.m_wfc_block.m_auto_solve = true;
		return;
	}

	bool positive = m_blocks[coord + ivec2(0, 1)] == nullptr;
	TileBlock& neighbour = positive ? *m_blocks[coord + ivec2(0, -1)] : *m_blocks[coord + ivec2(0, 1)];

	for(size_t x = 0; x < block.m_wfc_block.m_tiles.m_x; ++x)
	for(size_t y = 0; y < block.m_wfc_block.m_tiles.m_y; ++y)
	{
		size_t z = positive ? 0U : block.m_wfc_block.m_tiles.m_z - 1;
		size_t adjacent_z = positive ? block.m_wfc_block.m_tiles.m_z - 1 : 0U;
		uint16_t tile = neighbour.m_wfc_block.m_tiles.at(x, y, adjacent_z);
		block.m_wfc_block.m_wave.set_tile({ uint(x), uint(y), uint(z) }, tile);
	}

	block.m_wfc_block.m_wave.propagate();

	block.m_wfc_block.m_auto_solve = true;
}

void TileWorld::open_blocks(GfxSystem& gfx_system, const vec3& position, const ivec2& radius)
{
	ivec2 coord = ivec2(to_xz(position / m_block_size));

	for(int x = coord.x - radius.x; x < coord.x + radius.x + 1; ++x)
	for(int y = coord.y - radius.y; y < coord.y + radius.y + 1; ++y)
	{
		ivec2 neighbour = { x, y };
		if(!m_blocks[neighbour])
			this->generate_block(gfx_system, neighbour);
	}
}

const vec3 Human::muzzle_offset = { 0.f, 1.6f, -1.f };
float Human::headlight_angle = 40.f;
//float Human::headlight_angle = 30.f;
//float Human::headlight_angle = 20.f;

Human::Human(Id id, Entity& parent, const vec3& position, Faction faction)
	: Complex(id, type<Human>(), m_movable, m_emitter, m_receptor, m_active, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_movable(m_entity)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_faction(faction)
	, m_walk(false)
	, m_solid(m_entity, *this, CollisionShape(Capsule(0.35f, 1.1f), Y3 * 0.9f), SolidMedium::me(), CM_SOLID, false, 1.f)
{
	m_entity.m_world.add_task(this, short(Task::State)); // TASK_GAMEOBJECT

	m_emitter.addSphere(VisualMedium::me(), 0.1f);
	m_receptor.addSphere(VisualMedium::me(), 10.f);

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Human::~Human()
{
	m_entity.m_world.remove_task(this, short(Task::State));
}

void Human::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick);
	vec3 velocity = m_solid->linear_velocity();
	vec3 force = rotate(m_entity.m_rotation, m_force);

	m_solid->set_linear_velocity({ force.x, velocity.y - 1.f, force.z });
	m_solid->set_angular_velocity(m_torque);

	m_solid->set_angular_factor(Zero3);

	m_visor = this->aim();

	for(Bullet& bullet : reverse_adapt(m_bullets))
	{
		bullet.m_age += delta;
		if(bullet.m_age > 20)
			m_bullets.pop_back();
	}

	bool ia = m_faction == Faction::Ennemy;
	if(ia && m_life > 0.f)
	{
		m_target = nullptr;

		ReceptorScope* vision = m_receptor.scope(VisualMedium::me());
		for(Entity* entity : vision->m_scope.store())
			if(entity->isa<Human>())
			{
				Human& human = entity->part<Human>();
				if(human.m_faction != m_faction && human.m_life > 0.f)
					m_target = &entity->part<Human>();
			}

		m_cooldown = max(0.f, m_cooldown - float(delta) * 0.01f);
		if(m_target)
		{
			m_movable.m_linear_velocity = Zero3;
			m_dest = Zero3;

			m_entity.set_rotation(look_at(m_entity.m_position, m_target->m_entity.m_position));
			if(m_cooldown == 0.f)
			{
				this->shoot();
				m_cooldown = 1.f;
			}
		}
		else
		{
			auto to_ray = [](const vec3& pos, const vec3& dir) -> Ray { return{ pos, pos + dir * 1000.f, dir, 1.f / dir }; };
			auto is_walkable = [&](const vec3& pos) { return m_entity.m_world.part<PhysicWorld>().ground_point(to_ray(pos, -Y3)) != Zero3; };

			if(m_dest == Zero3)
			{
				float amplitude = 10.f;
				m_dest = m_entity.m_position + vec3(random_scalar(-amplitude, amplitude), 0.f, random_scalar(-amplitude, amplitude));
				if(!is_walkable(m_dest))
					m_dest = Zero3;
			}

			if(m_dest != Zero3)
			{
				if(steer_2d(m_movable, m_dest, 3.f, float(delta) * float(c_tick_interval), 1.f))
				{
					m_movable.m_linear_velocity = Zero3;
					m_dest = Zero3;
				}
			}
		}
	}
}

quat Human::sight()
{
	return rotate(m_entity.m_rotation, X3, m_vangle);
}

Aim Human::aim()
{
	quat rotation = this->sight();
	vec3 muzzle = m_entity.m_position + rotate(m_entity.m_rotation, Human::muzzle_offset);
	vec3 end = muzzle + rotate(rotation, -Z3) * 30.f;

	Collision hit = m_entity.m_world.part<PhysicWorld>().raycast({ muzzle, end }, CM_GROUND | CM_SOLID);
	return { rotation, muzzle, hit.m_second ? hit.m_hit_point : end, hit.m_second ? &hit.m_second->m_entity : nullptr };
}

void Human::shoot()
{
	Aim aim = this->aim();
	m_bullets.push_back({ aim.start, aim.rotation, aim.end, 0 });

	if(aim.hit && aim.hit->isa<Human>())
	{
		Human& shot = aim.hit->part<Human>();
		shot.damage(1.f);
	}

	m_solid->impulse(rotate(m_entity.m_rotation, Z3 * 4.f), Zero3);
}

void Human::damage(float amount)
{
	if(m_shield) return;

	m_life -= amount;
	if(m_life <= 0.f)
	{
		m_headlight = false;
		m_force = Zero3;
		m_torque = Zero3;
		m_movable.m_linear_velocity = Zero3;
	}
}

Lamp::Lamp(Id id, Entity& parent, const vec3& position)
	: Complex(id, type<Lamp>(), m_movable, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_movable(m_entity)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Crate::Crate(Id id, Entity& parent, const vec3& position, const vec3& extents)
	: Complex(id, type<Crate>(), m_movable, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_movable(m_entity)
	, m_extents(extents)
	, m_solid(m_entity, *this, Cube(extents), SolidMedium::me(), CM_SOLID, false, 10.f)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Player::Player(TileWorld& world)
	: m_world(&world)
{}

void Player::spawn(const vec3& start_position)
{
	vec3 position = start_position + Y3 * 2.f * m_world->m_center_block->m_wfc_block.m_scale.y;
	m_human = &m_world->m_world.origin().construct<Human>(position, Faction::Ally);
	//m_human->m_headlight = false;
	m_human->m_stealth = true;
	m_human->m_walk = false;
}

void paint_bullet(Gnode& parent, Bullet& bullet)
{
	static ParticleGenerator* flash = parent.m_scene->m_gfx_system.particles().file("flash");
	static ParticleGenerator* trail = parent.m_scene->m_gfx_system.particles().file("impact");
	static ParticleGenerator* impact = parent.m_scene->m_gfx_system.particles().file("impact");

	Gnode& source = gfx::node(parent, {}, bullet.m_source, bullet.m_rotation);
	gfx::particles(source, *flash);

	toy::sound(source, "schklatweom", false);

	float t = float(bullet.m_age) / float(10);
	bool impacted = bullet.m_age > 10;

	if(!impacted)
	{
		Gnode& projectile = gfx::node(parent, {}, lerp(bullet.m_source, bullet.m_impact, t), bullet.m_rotation);
		gfx::shape(projectile, Cube(vec3(0.02f, 0.02f, 0.4f)), Symbol(Colour::None, Colour(1.f, 2.f, 1.5f)));
		gfx::particles(projectile, *trail);
	}

	if(impacted)
	{
		Gnode& hit = gfx::node(parent, {}, bullet.m_impact, bullet.m_rotation);
		gfx::particles(hit, *impact);
	}
}

void paint_lamp(Gnode& parent, Lamp& lamp)
{
	gfx::shape(parent, Sphere(0.1f), Symbol(Colour::None, Colour::Red));
	gfx::light(parent, LightType::Point, false, Colour(1.f, 0.3f, 0.2f), 10.f);
}

Material& highlight_material(const std::string& name, const Colour& colour, int factor)
{
	Material& material = Material::ms_gfx_system->fetch_material(name.c_str(), "pbr/pbr");
	material.m_pbr_block.m_enabled = true;
	material.m_pbr_block.m_albedo.m_value = Colour::Black;
	material.m_pbr_block.m_emissive.m_value = colour;
	material.m_pbr_block.m_emissive.m_value.m_a = float(factor);
	return material;
}

Model& human_model_glow(GfxSystem& gfx_system)
{
	//Material& glow_material = highlight_material("JointsGlow", Colour(0.2f, 0.8f, 2.4f), 2);
	static Material& glow_material = highlight_material("JointsGlow", Colour::Red, 4);
	static Model& human = *gfx_system.models().file("human00");
	static Model& model = model_variant(gfx_system, human, "human_glow", carray<cstring, 1>{ "Joints" },
																		 carray<Material*, 1>{ &glow_material });
	return model;
}

Model& human_model_stealth(GfxSystem& gfx_system)
{
	static Material& stealth_material = highlight_material("JointsStealth", Colour(0.2f, 0.2f, 0.2f), 2);
	static Model& human = *gfx_system.models().file("human00");
	static Model& model = model_variant(gfx_system, human, "human_stealth", carray<cstring, 1>{ "Joints" },
																		    carray<Material*, 1>{ &stealth_material });
	return model;
}

void paint_human(Gnode& parent, Human& human)
{
	static Model& model_normal = *parent.m_scene->m_gfx_system.models().file("human00");
	static Model& model_stealth = human_model_stealth(parent.m_scene->m_gfx_system);
	static Model& model_glow = human_model_glow(parent.m_scene->m_gfx_system);

	//Model& model = human.m_stealth ? model_stealth : (human.m_life <= 0.f ? model_normal : model_glow);
	Model& model = human.m_stealth ? model_stealth : model_glow;
	
	//glow_material.m_pbr_block.m_emissive.m_value.m_a;

	Gnode& self = gfx::node(parent, Ref(&human), human.m_entity.m_position, human.m_entity.m_rotation);
	
	Item& item = gfx::item(self, model, ITEM_SELECTABLE);
	Animated& animated = gfx::animated(self, item);

	struct State { std::string name; bool loop; };
	State state = { "IdleAim", true };
	if(human.m_force != Zero3 || human.m_torque != Zero3 || human.m_movable.m_linear_velocity != Zero3)
		state = { human.m_walk ? "Walk" : "RunAim", true };
	if(human.m_life <= 0.f)
		state = { "Die", false };

	if(animated.m_playing.empty() || animated.playing() != state.name)
		animated.play(state.name.c_str(), state.loop, 0.f, human.m_walk ? 0.7f : 1.f);

	Gnode& visor = gfx::node(parent, {}, human.m_visor.end);
	gfx::shape(visor, Circle(0.01f, Axis::Z), Symbol(Colour::White * 3.f, Colour::None, true), ITEM_BILLBOARD);

	auto bone_pose = [](Bone& bone)
	{
		return bxrotation(angle_axis(-c_pi * 0.5f, X3)) * bxscale(vec3(0.009999999776482582f)) * bone.m_pose;
	};

#if 0
	for(Bone& bone : animated.m_rig.m_skeleton.m_bones)
	{
		mat4 pose = bxrotation(human.m_entity.m_rotation) * bone_pose(bone);
		Gnode& node = gfx::node(self, {}, human.m_entity.m_position + vec3(pose * vec4(Zero3, 1.f)));
		gfx::shape(node, Sphere(0.02f), Symbol());
	}
#endif

	Bone* bone = animated.m_rig.m_skeleton.find_bone("RightHand");

	mat4 pose = bxrotation(human.m_entity.m_rotation) * bone_pose(*bone);
	Gnode& arm = gfx::node(self, {}, human.m_entity.m_position + vec3(pose * vec4(Zero3, 1.f)), human.m_entity.m_rotation);
	gfx::model(arm, "rifle");

	if(human.m_shield)
	{
		static Clock clock;
		float intensity = remap_trig(sin(clock.read() * 2.f), 0.8f, 1.f);

		Gnode& center = gfx::node(parent, Ref(&human), human.m_entity.m_position + rotate(human.m_entity.m_rotation, Y3));
		gfx::shape(center, Sphere(1.f), Symbol(Colour(0.2f, 0.8f, 2.4f) * intensity));
		gfx::light(center, LightType::Point, false, Colour(0.3f, 0.4f, 1.f) * intensity, 10.f);
		//toy::sound(parent, "grzzt");
		toy::sound(parent, "electricfield");
	}

	if(human.m_headlight)
	{
		Gnode& headlight = gfx::node(parent, Ref(&human), human.m_entity.m_position + rotate(human.m_entity.m_rotation, Human::muzzle_offset), human.sight());
		Light& light = gfx::light(headlight, LightType::Spot, false, Colour::White, 30.f);
		light.m_spot_angle = Human::headlight_angle;
		light.m_spot_attenuation = 0.9f;
	}

	for(Bullet& bullet : human.m_bullets)
		paint_bullet(self, bullet);
}

void paint_block(Gnode& parent, TileBlock& block)
{
	paint_tileblock(parent, Ref(&block.m_entity), block.m_wfc_block);
	if(!block.m_world_page.m_build_geometry)
		block.m_world_page.m_build_geometry = [&](WorldPage& page) { build_block_geometry(*parent.m_scene, page, block); };
}

Material& plain_material(GfxSystem& gfx_system, cstring name, const Colour& colour)
{
	Material& material = gfx_system.fetch_material("crate", "pbr/pbr");
	material.m_pbr_block.m_enabled = true;
	material.m_pbr_block.m_albedo.m_value = colour;
	return material;
}

void paint_crate(Gnode& parent, Crate& crate)
{
	static Material& material = plain_material(parent.m_scene->m_gfx_system, "crate", Colour::White);
	gfx::shape(parent, Cube(crate.m_extents), Symbol(Colour::None, Colour::White), 0U, &material);
	//Gnode& node = gfx::node(parent, {}, parent.m_attach->m_position, parent.m_attach->m_rotation, vec3(0.02f) * crate.m_extents);
	//gfx::model(node, "crate");
	//gfx::model(node, "futuristic_crate");
}

void paint_scene(Gnode& parent)
{
	//toy::sound(parent, "alarm", true);

	//Light& light = gfx::directional_light_node(parent);
	//light.m_colour = { 0.3f, 0.1f, 0.1f };

#ifdef TOY_SOUND
	parent.m_sound_manager->threadUpdate();
#endif

	parent.m_scene->m_environment.m_radiance.m_energy = 0.2f;
	parent.m_scene->m_environment.m_radiance.m_ambient = 0.04f;

	gfx::radiance(parent, "radiance/tiber_1_1k.hdr", BackgroundMode::None);
}

void paint_viewer(Viewer& viewer)
{
#ifndef MUD_PLATFORM_EMSCRIPTEN
	viewer.m_filters.m_glow.m_enabled = true;
	viewer.m_filters.m_glow.m_levels_1_4 = { 1.f, 0.f, 0.f, 0.f };
	viewer.m_filters.m_glow.m_bicubic_filter = true;
	viewer.m_filters.m_glow.m_intensity = 0.4f;
#endif

	Fog& fog = viewer.m_scene->m_environment.m_fog;

	static bool once = false;
	if(!once)
	{
		//fog.m_enabled = true;
		fog.m_colour = Colour::Red * 0.3f;
		fog.m_density = 0.01f;
		fog.m_height_curve = 0.1f;
		fog.m_depth_begin = 50.f;
		once = true;
	}
}

void physic_painter(GameScene& scene)
{
	static PhysicDebugDraw physic_draw = { *scene.m_scene->m_scene.m_immediate };

	scene.m_scene->painter("PhysicsDebug", [&](size_t index, VisuScene& visu_scene, Gnode& parent) {
		physic_draw.draw_physics(parent, *scene.m_game.m_world, VisualMedium::me());
	});
}

template <class T, class T_Store>
inline void range_entity_painter(VisuScene& scene, Entity& reference, float range, cstring name, T_Store& entities, void(*paint_func)(Gnode&, T&))
{
	auto paint = [&scene, &reference, range, &entities, paint_func](size_t index, VisuScene&, Gnode& parent)
	{
		float range2 = range*range;
		for(Entity* entity : entities.store())
		{
			float dist2 = distance2(entity->m_position, reference.m_position);
			if(dist2 < range2 && entity->isa<T>())
				paint_func(scene.entity_node(parent, *entity, index), entity->part<T>());
		}
	};
	scene.m_painters.emplace_back(make_unique<VisuPainter>(name, scene.m_painters.size(), paint));
}

void ex_platform_scene(GameShell& app, GameScene& scene)
{
	static OmniVision vision = { *scene.m_game.m_world };
	scene.m_camera.m_entity.set_position(Zero3);

	//scene_painters(*scene.m_scene, vision.m_store);
	scene.m_scene->painter("World", [&](size_t index, VisuScene& scene, Gnode& parent) {
		UNUSED(scene); paint_scene(parent.sub((void*)index));
	});
	
	Entity& reference = val<Player>(scene.m_player).m_human->m_entity;

	range_entity_painter<Lamp>(*scene.m_scene, reference, 100.f, "Lamps", vision.m_store, paint_lamp);
	range_entity_painter<Human>(*scene.m_scene, reference, 100.f, "Humans", vision.m_store, paint_human);
	range_entity_painter<Crate>(*scene.m_scene, reference, 100.f, "Crates", vision.m_store, paint_crate);
	range_entity_painter<TileBlock>(*scene.m_scene, reference, 200.f, "Tileblocks", vision.m_store, paint_block);

	//physic_painter(*scene.m_scene);
}

vec3 find_fitting_player_location(WfcBlock& tileblock)
{
	vec3 center = vec3(tileblock.m_size) * 0.5f;
	vec3 start_coord = {};
	for(size_t i = 0; i < tileblock.m_tiles.size(); ++i)
		if(tileblock.m_tileset.m_tiles_flip[tileblock.m_tiles[i]].m_name == "empty_covered")
		{
			vec3 coord = { tileblock.m_tiles.x(i), tileblock.m_tiles.y(i), tileblock.m_tiles.z(i) };
			if(distance2(coord, center) < distance2(start_coord, center))
				start_coord = coord;
		}
	return tileblock.to_position(start_coord);
}

Style& menu_style()
{
	static Style style("GameMenu", styles().wedge, [](Layout& l) { l.m_space = { PARAGRAPH, SHRINK, SHRINK };
																   l.m_align = { CENTER, CENTER };
																   l.m_padding = vec4(15.f);
																   l.m_spacing = vec2(10.f); });
	return style;
}

Style& button_style()
{
	static Style style("GameButton", styles().button, [](Layout& l) {},
													  [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::AlphaWhite; s.m_text_size = 24.f; },
													  [](Style& s) { s.decline_skin(HOVERED).m_text_colour = Colour::White; });
	return style;
}

struct KeyMove
{
	KeyCode key;
	vec3 force;
};

void human_control_key(Widget& widget, vec3& force, vec3& torque, const KeyMove& move)
{
	const float speed = widget.root_sheet().m_keyboard.m_shift ? 4.f : 15.f;
	const float rotation = 4.f;

	if(widget.key_event(move.key, EventType::Pressed))
		force += move.force * speed;
	if(widget.key_event(move.key, EventType::Released))
		force -= move.force * speed;
}

static void human_velocity_controller(Widget& widget, Human& human)
{
	bool shift = widget.root_sheet().m_keyboard.m_shift;

	const KeyMove moves[8] =
	{
		{ KC_UP,    -Z3 },{ KC_W, -Z3 },
		{ KC_DOWN,   Z3 },{ KC_S,  Z3 },
		{ KC_LEFT,	-X3 },{ KC_A, -X3 },
		{ KC_RIGHT,  X3 },{ KC_D,  X3 },
	};

	for(const KeyMove& key_move : moves)
		human_control_key(widget, human.m_force, human.m_torque, key_move);

	if(widget.key_event(KC_SPACE, EventType::Stroked))
		human.m_solid->impulse(Y3 * 16.f, Zero3);
}

void ex_platform_game_hud(Viewer& viewer, GameScene& game, Human& human)
{
	Widget& overlay = ui::screen(viewer);

	bool isometric = false;// true;
	OrbitController& orbit = isometric ? ui::isometric_controller(viewer)
									   : ui::orbit_controller(viewer);

	orbit.set_position(human.m_entity.m_position + Y3 * 2.f);

	if(!isometric)
		orbit.update_position(human.sight());

	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::Mouse, EventType::Moved))
	{
		const float rotation_speed = 1.f;
		vec2 angle = -mouse_event.m_delta / 250.f * rotation_speed;

		human.m_entity.rotate(Y3, angle.x);
		human.m_vangle += angle.y;
		human.m_vangle = min(c_pi / 2.f - 0.01f, max(-c_pi / 2.f + 0.01f, human.m_vangle));
	}

	if(human.m_life > 0.f)
	{
		human_velocity_controller(viewer, human);

		if(viewer.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
		{
			human.shoot();
		}

		if(viewer.key_event(KC_LSHIFT, EventType::Pressed))
			human.m_headlight = !human.m_headlight;

		if(viewer.key_event(KC_LCONTROL, EventType::Pressed))
			human.m_shield = !human.m_shield;

		ui::fill_bar(overlay, human.m_life);
	}

	if(human.m_life <= 0.f)
	{
		Widget& modal = ui::popup(overlay, styles().modal, ui::PopupFlags::None);
		ui::title(modal, "You are DEAD");
		ui::label(modal, "Press ENTER to try again");

		if(viewer.key_event(KC_RETURN, EventType::Stroked))
		{
			human.m_life = 5.f;
		}
	}
}

void ex_platform_game_ui(Widget& parent, GameScene& game)
{
	Widget& self = ui::widget(parent, styles().board, &game);//ui::board(parent);

	Viewer& viewer = ui::viewer(self, game.m_scene->m_scene);
	game.m_viewer = &viewer;
	//viewer.take_modal();

	//Viewer& secondary_viewer = ui::viewer(self, game.m_scene->m_scene);
	//ui::isometric_controller(viewer);

	Player& player = val<Player>(game.m_player);
	if(player.m_human)
		ex_platform_game_hud(viewer, game, *player.m_human);
}

Viewer& ex_platform_menu_viewport(Widget& parent, GameShell& app)
{
	Viewer& viewer = ui::scene_viewer(parent);
	Gnode& scene = viewer.m_scene->begin();

#ifdef TOY_SOUND
	scene.m_sound_manager = app.m_visu_system->m_sound_system.get();
#endif

	paint_viewer(viewer);
	paint_scene(scene);
	
	static Model& human = human_model_glow(viewer.m_scene->m_gfx_system);
	static Clock clock;

	Gnode& node = gfx::node(scene, {}, -Y3 * 0.4f, angle_axis(fmod(clock.read(), 2.f * c_pi), Y3), Unit3 * 0.5f);
	Item& item = gfx::item(node, human);
	Animated& animated = gfx::animated(node, item);
	 
	if(animated.m_playing.empty())
		animated.play("IdleAim", true, 0.f);

	toy::sound(node, "complexambient", true);

	return viewer;
}

void ex_platform_menu(Widget& parent, Game& game)
{
	Widget& self = ui::board(parent);

	Viewer& viewer = ex_platform_menu_viewport(self, *game.m_shell);
	Widget& overlay = ui::screen(viewer);

	Widget& menu = ui::widget(overlay, menu_style());

	if(ui::button(menu, button_style(), "Start game").activated())
	{
		ex_platform_start(*game.m_shell, game);
	}

	ui::button(menu, button_style(), "Continue game");
	ui::button(menu, button_style(), "Quit");
}

void ex_platform_pump_game(GameShell& app, Game& game, Widget& parent)
{
	Player& player = val<Player>(game.m_player);
	TileWorld& world = as<TileWorld>(game.m_world->m_complex);
	TileBlock& block = *world.m_center_block;

	world.next_frame();

	if(player.m_human == nullptr)
	{
		Viewer& viewer = ex_platform_menu_viewport(parent, *game.m_shell);
		Widget& overlay = ui::screen(viewer);

		Widget& dialog = ui::widget(overlay, menu_style());
		ui::item(dialog, button_style(), "Loading");

		if(block.m_wfc_block.m_wave_solved > 0)
		{
			vec3 start_position = find_fitting_player_location(block.m_wfc_block);
			player.spawn(start_position);
		}
	}
	else
	{
		static GameScene& scene = app.add_scene();
		scene.m_scene->next_frame();

		ex_platform_game_ui(parent, scene);

		vec3 position = player.m_human->m_entity.m_position;
		world.open_blocks(*app.m_visu_system->m_gfx_system, position, { 0, 1 });
	}
}

void ex_platform_init(GameShell& app, Game& game)
{
	UNUSED(game);
	app.m_visu_system->m_gfx_system->add_resource_path("examples/ex_platform/");
	app.m_visu_system->m_gfx_system->add_resource_path("examples/05_character/");
	app.m_visu_system->m_gfx_system->add_resource_path("examples/17_wfc/");
}

void ex_platform_start(GameShell& app, Game& game)
{
	GlobalPool::me().create_pool<TileBlock>();
	GlobalPool::me().create_pool<TileWorld>();
	GlobalPool::me().create_pool<Human>();
	GlobalPool::me().create_pool<Crate>();
	GlobalPool::me().create_pool<OCamera>();

	TileWorld& tileworld = GlobalPool::me().pool<TileWorld>().construct("Arcadia");
	game.m_world = &tileworld.m_world;

	app.m_core->section(0).add_task(game.m_world);

	//app.m_context->lock_mouse(true);

	static Player player = { tileworld };
	game.m_player = Ref(&player);

	tileworld.generate_block(*app.m_visu_system->m_gfx_system, ivec2(0));
}

void ex_platform_pump(GameShell& app, Game& game, Widget& parent, Dockbar* dockbar = nullptr)
{
	if(!game.m_player)
	{
		ex_platform_menu(parent, game);
	}
	else
	{
		ex_platform_pump_game(app, game, parent);
	}
}

void ex_platform_pump(GameShell& app, Game& game)
{
#ifdef _PLATFORM_TOOLS
	edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_platform_pump(app, game, *app.m_editor.m_screen, app.m_editor.m_dockbar);
#else
	ex_platform_pump(app, game, game.m_screen ? *game.m_screen : app.m_ui->begin());
#endif
}

#ifdef _EX_PLATFORM_EXE
int main(int argc, char *argv[])
{
	cstring example_path = TOY_RESOURCE_PATH "examples/ex_platform/";
	cstring human_path = TOY_RESOURCE_PATH "examples/05_character/";
	cstring wfc_path = TOY_RESOURCE_PATH "examples/17_wfc/";
	GameShell app(carray<cstring, 4>{ TOY_RESOURCE_PATH, example_path, human_path, wfc_path }, argc, argv);
	
	GameModule module = { _platform::m(), &ex_platform_init, &ex_platform_start, &ex_platform_pump, &ex_platform_scene };
	app.run_game(module);
}
#endif
