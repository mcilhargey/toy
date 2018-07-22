#include <blocks/ex_blocks.h>
#include <05_character/05_character.h>
#include <toy/toy.h>

#include <blocks/Api.h>
#include <meta/blocks/Module.h>
#include <meta/05_character/Module.h>

#include <shell/Shell.h>

//#define _BLOCKS_TOOLS

using namespace mud;
using namespace toy;

Material& highlight_material(const std::string& name, const Colour& colour, int factor)
{
	Material& material = Material::ms_gfx_system->fetch_material(name.c_str(), "pbr/pbr");
	material.m_pbr_block.m_enabled = true;
	material.m_pbr_block.m_emissive.m_value = colour;
	material.m_pbr_block.m_emissive.m_value.m_a = float(factor);
	return material;
}

Faction::Faction(uint32_t id, const Colour& colour, short int energy)
	: m_id(index(type<Faction>(), id, Ref(this)))
	, m_colour(colour)
	, m_energy(energy)
{
	auto highlight_material = [=](int factor) -> Material&
	{
		return ::highlight_material("faction_" + to_string(m_id)  + "_highlight_" + to_string(factor), colour, factor);
	};

	m_highlight2 = &highlight_material(2);
	m_highlight11 = &highlight_material(11);
}

Faction::~Faction()
{
	unindex(type<Faction>(), m_id);
}

Well::Well(Id id, Entity& parent, const vec3& position)
	: Complex(id, type<Tank>(), m_emitter, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_emitter(m_entity)
{
	m_entity.m_world.add_task(this, short(Task::State)); // TASK_GAMEOBJECT

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Well::~Well()
{
	m_entity.m_world.remove_task(this, short(Task::State));
}

void Well::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick); UNUSED(delta);
}

Camp::Camp(Id id, Entity& parent, const vec3& position, Faction& faction)
	: Complex(id, type<Camp>(), *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_faction(faction)
	, m_position(position)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Shield::Shield(Id id, Entity& parent, const vec3& position, Faction& faction, float radius)
	: Complex(id, type<Shield>(), m_emitter, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_faction(faction)
	, m_radius(radius)
	, m_charge(1.f)
	, m_discharge(0.f)
	, m_solid(m_entity, *this, Sphere(radius), SolidMedium::me(), CollisionGroup(faction.m_energy), false, 0.f)
{
	m_entity.m_world.add_task(this, short(Task::State)); // TASK_GAMEOBJECT

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Shield::~Shield()
{
	m_entity.m_world.remove_task(this, short(Task::State));
}

void Shield::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick); UNUSED(delta);
	if(m_discharge > 0.f)
	{
		m_charge = max(0.f, m_charge - m_discharge);
		m_discharge = max(0.f, m_discharge - min(0.1f, m_discharge));
	}

	m_charge = min(1.f, m_charge + 0.01f);
}

Slug::Slug(Entity& parent, const vec3& source, const quat& rotation, const vec3& velocity, short int energy)
	: Complex(0, type<Slug>())
	, m_entity(0, *this, parent, source, rotation)
	, m_source(source)
	, m_velocity(velocity)
	, m_energy(energy)
	//, m_solid(m_entity, *this, Sphere(0.1f), SolidMedium::me(), CollisionGroup(energy), false, 1.f)
	, m_collider(m_entity, *this, Sphere(0.1f), SolidMedium::me(), CollisionGroup(energy))
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Slug::~Slug()
{}

void Slug::update()
{
	if(m_impacted)
		return;

	short int mask = CM_SOLID | CM_GROUND | CM_ENERGY_0 | CM_ENERGY_1 | CM_ENERGY_2;
	mask &= ~m_energy; // don't collide with our own energy

	//Collision collision = m_solid.m_impl->raycast(m_entity.m_position + m_velocity, mask);
	Collision collision = m_collider.m_impl->raycast(m_entity.m_position + m_velocity, mask);
	Entity* hit = collision.m_second ? &collision.m_second->m_entity : nullptr;

	if(hit != nullptr)
	{
		if(hit->isa<Tank>())
		{
			m_impacted = true;
			m_impact = collision.m_hit_point;

			hit->part<Tank>().m_control = false;
			hit->part<Tank>().m_hitpoints -= 25.f;

			if(hit->part<Tank>().m_hitpoints < 0.f)
				hit->part<Tank>().m_solid.m_impl->impulse(Y3 * 100.f, Zero3);
			else
				hit->part<Tank>().m_solid.m_impl->impulse(m_velocity * 10.f, Zero3);
		}

		if(hit->isa<Shield>())
		{
			auto reflect = [](const vec3& I, const vec3& N) { return I - 2.f * dot(N, I) * N; };

			vec3 N = normalize(collision.m_hit_point - hit->part<Shield>().m_entity.m_position);
			m_velocity = reflect(m_velocity, N);

			hit->part<Shield>().m_discharge += 1.0f;
		}
	}

	if(distance(m_entity.m_position, Zero3) > 1000.f)
		m_destroy = true;

	m_entity.set_position(m_entity.m_position + m_velocity);
	//m_collider.update_transform();
}

Tank::Tank(Id id, Entity& parent, const vec3& position, Faction& faction)
	: Complex(id, type<Tank>(), m_movable, m_emitter, m_active, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_movable(m_entity)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_faction(faction)
	, m_solid(m_entity, *this, CollisionShape(Cube(vec3(2.0f, 1.1f, 3.2f)), Y3 * 1.1f), SolidMedium::me(), CM_SOLID, false, 4.f)
{
	m_entity.m_world.add_task(this, short(Task::State)); // TASK_GAMEOBJECT

	m_emitter.addSphere(VisualMedium::me(), 0.1f);
	m_receptor.addSphere(VisualMedium::me(), 100.f);

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);
}

Tank::~Tank()
{
	m_entity.m_world.remove_task(this, short(Task::State));
}

void Tank::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick);

	vec3 linvel = m_solid.m_impl->linear_velocity();
	bool moving = any(greater(linvel, vec3(0.01f)));
	if(!m_control && !moving)
		m_control = true;

	//m_solid.m_impl->set_force(rotate(m_entity.m_rotation, m_force));
	//m_solid.m_impl->set_torque(m_torque);

	m_energy = min(100.f, m_energy + delta * .1f);

	for(auto& slug : reverse_adapt(m_slugs))
	{
		slug->update();
		if(slug->m_destroy)
			vector_remove_pt(m_slugs, *slug);
	}

	bool ia = m_faction.m_id != 1;
	if(!ia)
		return;

	if(m_control && m_hitpoints > 0.f)
	{
		m_target = nullptr;

		ReceptorScope* vision = m_receptor.scope(VisualMedium::me());
		for(Entity* entity : vision->m_scope.store())
			if(entity->isa<Tank>() && &entity->part<Tank>().m_faction != &m_faction)
			{
				m_target = &entity->part<Tank>();
			}

		m_cooldown = max(0.f, m_cooldown - float(delta) * 0.01f);
		if(m_target)
		{
			m_turret_angle = look_at(m_entity.m_position, m_target->m_entity.m_position, -Z3);

			float d = distance(m_target->m_entity.m_position, m_entity.m_position);
			if(d > m_range)
			{
				m_dest = m_target->m_entity.m_position;
			}
			else if(m_cooldown == 0.f)
			{
				this->shoot();
				m_dest = Zero3;
				m_cooldown = 1.f;
			}
		}

		if(m_dest != Zero3)
		{
			if(steer_2d(m_movable, m_dest, 15.f, float(delta) * float(c_tick_interval), m_range))
				m_dest = Zero3;
		}
	}
	else
	{
		m_movable.m_linear_velocity = Zero3;
		m_dest = Zero3;
	}
}

void Tank::shoot()
{
	static const vec3 tank_muzzle = { 0.f, 1.75f, -3.7f };

	if(m_energy < 10.f)
		return;

	m_energy -= 10.f;

	vec3 velocity = this->turret_direction() * 5.f;
	quat rotation = this->turret_rotation();

	m_slugs.emplace_back(make_object<Slug>(m_entity, m_entity.m_position + rotate(rotation, tank_muzzle), rotation, velocity, m_faction.m_energy));
}

BlockWorld::BlockWorld(const std::string& name)
	: Complex(0, type<BlockWorld>(), m_bullet_world, m_navmesh, *this)
	, m_world(0, *this, name)
	, m_bullet_world(m_world)
	, m_navmesh(m_world)
	//, m_block_subdiv(64, 1, 64)
	, m_block_subdiv(32, 1, 32)
	, m_tile_scale(10.f, 4.f, 10.f)
	, m_block_size(vec3(m_block_subdiv) * m_tile_scale)
	, m_world_size(m_block_size)
{
	TPool<Faction>& pool = GlobalPool::me().pool<Faction>();
	m_factions.push_back(&pool.construct(0, Colour::Red, CM_ENERGY_0));
	m_factions.push_back(&pool.construct(0, Colour::Pink, CM_ENERGY_1));
	m_factions.push_back(&pool.construct(0, Colour::Cyan, CM_ENERGY_2));
}

BlockWorld::~BlockWorld()
{
	m_world.destroy();
}

void BlockWorld::generate_block(GfxSystem& gfx_system, const ivec2& coord)
{
	static WaveTileset& tileset = generator_tileset(gfx_system);

	TileBlock& block = ::generate_block(gfx_system, tileset, m_world.origin(), coord, m_block_subdiv, m_tile_scale, false);

	block.m_world_page.m_geometry_filter = { "flat_low", "flat_high", "cliff_side_0_0", "cliff_corner_in_0_0", "cliff_corner_out_0_0" };

	m_blocks[coord] = &block;
	if(m_center_block == nullptr)
		m_center_block = &block;

	block.m_wfc_block.m_auto_solve = true;
}

Player::Player(BlockWorld& world)
	: m_world(&world)
	, m_tank(0, world.m_world.origin(), Y3 * 20.f, *world.m_factions[0])
{}

void paint_well(Gnode& parent, Well& well)
{
	UNUSED(parent); UNUSED(well);
}

Model& faction_fresnel_material(GfxSystem& gfx_system, Model& model)
{
	static Material& material = highlight_material("no_highlight", Colour::Black, 0);

	std::string name = model.m_name + "_dead";
	return model_variant(gfx_system, model, name.c_str(), carray<cstring, 2>{ "Highlight11", "Highlight2" }, 
														  carray<Material*, 2>{ &material, &material });
}

void paint_shield(Gnode& parent, Shield& shield)
{
	static Material* discharge = &parent.m_scene->m_gfx_system.fetch_material("shield_discharge", "fresnel");

	static std::vector<Material*> power = std::vector<Material*>(4);

	auto fresnel_material = [&](Material& material, Colour colour)
	{
		material.m_fresnel_block.m_enabled = true;
		material.m_fresnel_block.m_value.m_value = colour;
		material.m_fresnel_block.m_value.m_texture = parent.m_scene->m_gfx_system.textures().file("beehive.png");
	};

	if(power[shield.m_faction.m_id] == nullptr)
	{
		Material* material = &parent.m_scene->m_gfx_system.fetch_material(("shield_faction_" + to_string(shield.m_faction.m_id)).c_str(), "fresnel");
		fresnel_material(*material, shield.m_faction.m_colour * 4.f);
		power[shield.m_faction.m_id] = material;
	}

	if(shield.m_discharge > 0.f)
		toy::sound(parent, "bzwomb");

	static Clock clock;

	float period = remap_trig(sin(clock.read() * 2.f), 0.f, 0.05f);

	auto paint = [&](Material& material, float bias, Colour& colour)
	{
		fresnel_material(material, colour);
		material.m_fresnel_block.m_fresnel_bias = bias;
		gfx::shape(parent, Sphere(shield.m_radius), Symbol(Colour::None, colour), 0U, &material);
	};

	if(shield.m_discharge > 0.f)
	{
		paint(*discharge, shield.m_discharge, hsl_to_rgb(random_scalar(0.f, 1.f), 1.f, 0.5f) * shield.m_discharge * 10.f);
	}
	else
	{
		paint(*power[shield.m_faction.m_id], period, shield.m_faction.m_colour * 4.f * shield.m_charge);
	}
}

void paint_shell(Gnode& parent, Slug& shell)
{
	static ParticleGenerator* flash = parent.m_scene->m_gfx_system.particles().file("flash");
	static ParticleGenerator* trail = parent.m_scene->m_gfx_system.particles().file("impact");
	static ParticleGenerator* impact = parent.m_scene->m_gfx_system.particles().file("impact");

	Gnode& source = gfx::node(parent, Ref(&shell), shell.m_source, shell.m_entity.m_rotation);
	gfx::particles(source, *flash);

	bool active = toy::sound(source, "bang");

	if(!shell.m_impacted)
	{
		Gnode& projectile = gfx::node(parent, Ref(&shell), shell.m_entity.m_position, shell.m_entity.m_rotation);
		gfx::shape(projectile, Cube(vec3(0.4f, 0.4f, 1.f)), Symbol(Colour::None, Colour(1.f, 2.f, 1.5f)));
		gfx::particles(projectile, *trail);
	}

	if(shell.m_impacted)
	{
		Gnode& hit = gfx::node(parent, Ref(&shell), shell.m_impact, shell.m_entity.m_rotation);
		active |= !gfx::particles(hit, *impact).m_ended;
		active |= toy::sound(hit, "explode");
	}

	shell.m_destroy |= !active;
}

Model& faction_model_dead_variant(GfxSystem& gfx_system, Model& model)
{
	static Material& material = highlight_material("no_highlight", Colour::Black, 0);

	std::string name = model.m_name + "_dead";
	return model_variant(gfx_system, model, name.c_str(), carray<cstring, 2>{ "Highlight11", "Highlight2" }, 
														  carray<Material*, 2>{ &material, &material });
}

Model& faction_model_variant(GfxSystem& gfx_system, Faction& faction, Model& model)
{
	std::string name = model.m_name + "_faction" + to_string(faction.m_id);
	return model_variant(gfx_system, model, name.c_str(), carray<cstring, 2>{ "Highlight11", "Highlight2" }, 
														  carray<Material*, 2>{ faction.m_highlight11, faction.m_highlight2 });
}

void hud_bar(Gnode& parent, const vec3& position, const vec2& offset, float percentage, Colour colour)
{
	static const vec2 size = { 4.f, 0.2f };
	vec2 fill_offset = { size.x * -(1.f - percentage) / 2.f, 0.f };
	gfx::shape(parent, Quad(position, offset, vec2(4.f, 0.2f)), Symbol(Colour::White, Colour::None, true), ITEM_BILLBOARD);
	gfx::shape(parent, Quad(position, offset + fill_offset, vec2(4.f * percentage, 0.2f)), Symbol(Colour::None, colour, true, true), ITEM_BILLBOARD);
}

void paint_tank(Gnode& parent, Tank& tank)
{
	static Material* debug = &parent.m_scene->m_gfx_system.debug_material();
	static Material* stealth = &parent.m_scene->m_gfx_system.fetch_material("tank_stealth", "fresnel");
	{
		stealth->m_fresnel_block.m_enabled = true;
		stealth->m_fresnel_block.m_value.m_value = { 0.f, 0.3f, 2.f };
		//stealth->m_fresnel_block.m_value.m_texture = parent.m_scene->m_gfx_system.textures().file("beehive.png");
	}

	GfxSystem& gfx_system = parent.m_scene->m_gfx_system;

	static std::vector<Model*> tank_base_models = std::vector<Model*>(size_t(Faction::s_max_factions));
	static std::vector<Model*> tank_turret_models = std::vector<Model*>(size_t(Faction::s_max_factions));

	static Model& tank_base_dead = faction_model_dead_variant(gfx_system, *gfx_system.models().file("scifi_tank_base"));
	static Model& tank_turret_dead = faction_model_dead_variant(gfx_system, *gfx_system.models().file("scifi_tank_turret"));

	if(tank_base_models[tank.m_faction.m_id] == nullptr)
	{
		tank_base_models[tank.m_faction.m_id] = &faction_model_variant(gfx_system, tank.m_faction, *gfx_system.models().file("scifi_tank_base"));
		tank_turret_models[tank.m_faction.m_id] = &faction_model_variant(gfx_system, tank.m_faction, *gfx_system.models().file("scifi_tank_turret"));
	}

	Gnode& turret = gfx::node(parent, {}, parent.m_attach->m_position, tank.turret_rotation());

	if(tank.m_hitpoints > 0.f)
	{
		if(tank.m_stealth)
		{
			gfx::item(parent, tank_base_dead, 0U, stealth);
			gfx::item(turret, tank_turret_dead, 0U, stealth);
		}
		else
		{
			gfx::item(parent, *tank_base_models[tank.m_faction.m_id]);
			gfx::item(turret, *tank_turret_models[tank.m_faction.m_id]);
		}
	}
	else
	{
		gfx::item(parent, tank_base_dead);
		gfx::item(turret, tank_turret_dead);
	}

	enum States { Alive = 2, Dead = 3, Slugs = 4 };

	if(tank.m_hitpoints > 0.f)
	{
		Gnode& alive = parent.subx(Alive);
		Gnode& symbol = gfx::node(alive, {}, parent.m_attach->m_position, parent.m_attach->m_rotation);
		gfx::shape(alive, Torus(4.f, 0.1f), Symbol(Colour::None, tank.m_faction.m_colour * 2.f));
		//gfx::shape(alive, Circle(4.f), Symbol(tank.m_faction.m_colour));

		//Gnode& light = gfx::node(alive, {}, parent.m_attach->m_position + Y3 * 2.f, tank.turret_rotation());
		//gfx::light(light, LightType::Point, false, tank.m_faction.m_colour * 1.5f, 20.f, 0.4f);

		static const Colour energy = { 0.f, 0.6f, 1.f };
		static const Colour life = { 0.f, 1.f, 0.2f };

		if(false)
		{
			hud_bar(alive, Zero3, vec2(0.f), max(0.f, tank.m_hitpoints * 0.01f), life);
			hud_bar(alive, Zero3, vec2(0.f, -0.4f), max(0.f, tank.m_energy * 0.01f), energy);
		}
	}
	else
	{
		Gnode& dead = parent.subx(Dead);
		static ParticleGenerator* explode = parent.m_scene->m_gfx_system.particles().file("explode");

		gfx::particles(dead, *explode);
		toy::sound(dead, "explosion");
	}

	Gnode& slugs = parent.subx(Slugs);
	//for(auto& slug : tank.m_slugs)
	//	paint_shell(slugs.subi(slug.get()), *slug);
	for(size_t i = 0; i < tank.m_slugs.size(); ++i)
		paint_shell(slugs.subx(i), *tank.m_slugs[i]);
}

void paint_block_wire(Gnode& parent, Block& block)
{
	paint_block_wireframe(parent, block, Colour(1.f, 2.f, 1.5f));
}

void paint_block(Gnode& parent, TileBlock& block)
{
	paint_tileblock(parent, Ref(&block.m_entity), block.m_wfc_block);
}

#define TOY_GI 0

void paint_scene(Gnode& parent, bool radiance)
{
#ifdef TOY_SOUND
	parent.m_sound_manager->threadUpdate();
#endif

#ifndef MUD_PLATFORM_EMSCRIPTEN
	Light& light = gfx::directional_light_node(parent);
	light.m_shadow_range = 500.f;
#endif

	gfx::radiance(parent, "radiance/tiber_1_1k.hdr", radiance ? BackgroundMode::Radiance
															  : BackgroundMode::None);

#if TOY_GI
	GIProbe& probe = gfx::gi_probe(parent);
	//probe.debug_draw(parent, GIBaker::DEBUG_LIGHT);

	static size_t counter = 0;
	if(++counter == 50)
		probe.bake();
#endif
}

void paint_viewer(Viewer& viewer)
{
	viewer.m_camera.m_far = 500.f;

#ifndef MUD_PLATFORM_EMSCRIPTEN
	viewer.m_filters.m_glow.m_enabled = true;
	viewer.m_filters.m_glow.m_levels_1_4 = { 1.f, 1.f, 0.f, 0.f };
	viewer.m_filters.m_glow.m_bicubic_filter = true;
#endif
}

struct KeyMove
{
	KeyCode key;
	vec3 force;
	vec3 torque;
};

void tank_control_key(Widget& widget, vec3& force, vec3& torque, const KeyMove& move)
{
	if(widget.key_event(move.key, EventType::Pressed))
	{
		force += move.force;
		torque += move.torque * 2.f;
	}
	if(widget.key_event(move.key, EventType::Released))
	{
		force -= move.force;
		torque -= move.torque * 2.f;
	}
}

static void tank_velocity_controller(Widget& widget, Tank& tank)
{
	//bool shift = widget.root_sheet().m_keyboard.m_shift;

	const KeyMove moves[8] =
	{
		{ KC_UP,    -Z3, Zero3 }, { KC_W,  -Z3, Zero3 },
		{ KC_DOWN,   Z3, Zero3 }, { KC_S,   Z3, Zero3 },
		{ KC_LEFT,  Zero3,  Y3 }, { KC_A,  Zero3,  Y3 },
		{ KC_RIGHT, Zero3, -Y3 }, { KC_D,  Zero3, -Y3 },
	};

	for(const KeyMove& key_move : moves)
		tank_control_key(widget, tank.m_force, tank.m_torque, key_move);
}

Style& screen_style()
{
	static Style style = { "GameScreen", styles().wedge, [](Layout& l) { l.m_space = LAYOUT; l.m_padding = vec4(30.f); } };
	return style;
}

Style& right_panel_style(UiWindow& ui_window)
{
	static ImageSkin skin = { *ui_window.find_image("graphic/red_on"), 46, 28, 38, 30 };
	
	static Style style = { "GameMenu", styles().wedge, [](Layout& l) { l.m_space = UNIT; l.m_align = { RIGHT, CENTER }; l.m_padding = vec4(30.f); l.m_spacing = vec2(30.f); },
													   [](InkStyle& s) { s.m_empty = false; s.m_image_skin = skin; } };
	return style;
}

Style& center_panel_style(UiWindow& ui_window)
{
	static ImageSkin skin = { *ui_window.find_image("graphic/red_on"), 46, 28, 38, 30 };
	
	static Style style = { "GameMenu", styles().wedge, [](Layout& l) { l.m_space = UNIT; l.m_align = { CENTER, CENTER }; l.m_padding = vec4(30.f); l.m_spacing = vec2(30.f); },
													   [](InkStyle& s) { s.m_empty = false; s.m_image_skin = skin; } };
	return style;
}

Style& left_panel_style(UiWindow& ui_window)
{
	static ImageSkin skin = { *ui_window.find_image("graphic/red_on"), 46, 28, 38, 30 };
	
	static Style style = { "GameMenu", styles().wedge, [](Layout& l) { l.m_space = UNIT; l.m_align = { LEFT, CENTER }; l.m_padding = vec4(30.f); l.m_spacing = vec2(30.f); },
													   [](InkStyle& s) { s.m_empty = false; s.m_image_skin = skin; } };
	return style;
}

Style& menu_style()
{
	static Style style = { "GameMenu", styles().wedge, [](Layout& l) { l.m_space = UNIT; l.m_align = { LEFT, CENTER }; l.m_padding = vec4(120.f); l.m_padding.x = 240.f; l.m_spacing = vec2(30.f); } };
	return style;
}

Style& button_style(UiWindow& ui_window)
{
	static ImageSkin skin = { *ui_window.find_image("graphic/red_off"), 46, 28, 38, 30 };
	static ImageSkin skin_focused = { *ui_window.find_image("graphic/red_on"), 46, 28, 38, 30 };

	static Style style = { "GameButton", styles().button, [](Layout& l) { l.m_size = { 400.f, 80.f }; },
														  [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::White; s.m_text_font = "veramono"; s.m_text_size = 24.f; s.m_padding = vec4(40.f, 20.f, 40.f, 20.f); s.m_image_skin = skin; },
														  [](Style& s) { s.decline_skin(HOVERED).m_text_colour = Colour::White; s.decline_skin(HOVERED).m_image_skin = skin_focused; } };
	return style;
}

Style& label_style()
{
	static Style style = { "GameLabel", styles().button, [](Layout& l) {},
														 [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::White; s.m_text_font = "veramono"; s.m_text_size = 14.f;  } };
	return style;
}

void ex_blocks_game_ui(Widget& parent, GameScene& game)
{
	Player& player = val<Player>(game.m_player);

	Widget& self = ui::widget(parent, styles().board, &game);

	Viewer& viewer = ui::viewer(self, game.m_scene->m_scene);
	game.m_viewer = &viewer;

	OrbitController& orbit = ui::orbit_controller(viewer, 0.f, -c_pi / 4.f, 200.f);
	orbit.set_position(player.m_tank.m_entity.m_position);

#ifdef TOY_SOUND
	game.m_scene->m_snd_manager.m_listener.setTransform(viewer.m_camera.m_node.m_position, viewer.m_camera.m_node.m_rotation);
#endif

	Ray ray = viewer.mouse_ray();
	vec3 target = plane_segment_intersection(Plane(Y3, player.m_tank.m_entity.m_position.y), to_segment(ray));

	player.m_tank.m_turret_angle = look_at(player.m_tank.m_entity.m_position, target);

	tank_velocity_controller(viewer, player.m_tank);

	if(KeyEvent key_event = viewer.key_event(KC_LCONTROL))
		player.m_tank.m_stealth = !player.m_tank.m_stealth;

	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
	{
		viewer.take_focus();
		player.m_tank.shoot();
	}

	static vec3 destination = Zero3;

	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::MouseRight, EventType::Stroked))
	{
		viewer.take_focus();

		Ray pick_ray = viewer.m_viewport.ray(mouse_event.m_relative);
		destination = player.m_world->m_bullet_world.ground_point(pick_ray);
	}

	if(destination != Zero3)
	{
		static Clock clock;
		if(steer_2d(player.m_tank.m_movable, destination, 15.f, float(clock.step()), 0.1f))
			destination = Zero3;
	}

	paint_viewer(viewer);

	static Style& style_screen = screen_style();
	static Style& style_right_panel = right_panel_style(parent.ui_window());
	static Style& style_center_panel = center_panel_style(parent.ui_window());
	static Style& style_left_panel = left_panel_style(parent.ui_window());
	static Style& style_label = label_style();

	Widget& screen = ui::widget(viewer, style_screen);
	Widget& board = ui::board(screen);
	Widget& row = ui::row(screen);
	Widget& left_panel = ui::widget(row, style_left_panel);
	Widget& center_panel = ui::widget(row, style_center_panel);

	ui::item(left_panel, style_label, "Destroy All Enemy Shields");

	ui::icon(center_panel, "(World)");
}

Viewer& ex_blocks_menu_viewport(Widget& parent, GameShell& app)
{
	Viewer& viewer = ui::scene_viewer(parent);
	Gnode& scene = viewer.m_scene->begin();

#ifdef TOY_SOUND
	scene.m_sound_manager = app.m_visu_system->m_sound_system.get();
#endif

	paint_viewer(viewer);
	paint_scene(scene, false);

	static Model& tank_base = *viewer.m_scene->m_gfx_system.models().file("scifi_tank_base");
	static Model& tank_turret = *viewer.m_scene->m_gfx_system.models().file("scifi_tank_turret");

	static Clock clock;

	Gnode& node = gfx::node(scene, {}, -Y3 * 0.2f + X3 * 0.3f, angle_axis(fmod(clock.read() / 2.f, 2.f * c_pi), Y3), Unit3 * 0.2f);
	//gfx::shape(node, Cube(), Symbol());
	gfx::item(node, tank_base);
	gfx::item(node, tank_turret);
	
	//toy::sound(node, "complexambient", true);

	return viewer;
}

void ex_blocks_menu(Widget& parent, Game& game)
{
	static Style& style_button = button_style(parent.ui_window());
	static Style& style_menu = menu_style();

	Widget& self = ui::board(parent);

	Viewer& viewer = ex_blocks_menu_viewport(self, *game.m_shell);
	Widget& overlay = ui::screen(viewer);

	Widget& menu = ui::widget(overlay, style_menu);

	ui::icon(menu, "(toy)");

	if(ui::button(menu, style_button, "Start game").activated())
	{
		ex_blocks_start(*game.m_shell, game);
	}

	ui::button(menu, style_button, "Continue game");
	ui::button(menu, style_button, "Quit");
}

void ex_blocks_pump_game(GameShell& app, Game& game, Widget& parent)
{
	static GameScene& scene = app.add_scene();

	game.m_world->next_frame();
	scene.m_scene->next_frame();

	ex_blocks_game_ui(parent, scene);
}

void ex_blocks_init(GameShell& app, Game& game)
{
	UNUSED(game);
	app.m_visu_system->m_gfx_system->add_resource_path("examples/ex_blocks/");
}

void ex_blocks_start(GameShell& app, Game& game)
{
	GlobalPool::me().create_pool<BlockWorld>();
	GlobalPool::me().create_pool<Well>();
	GlobalPool::me().create_pool<Camp>();
	GlobalPool::me().create_pool<Shield>();
	GlobalPool::me().create_pool<Tank>();
	GlobalPool::me().create_pool<Fract>();
	GlobalPool::me().create_pool<Sector>();
	GlobalPool::me().create_pool<Block>();
	GlobalPool::me().create_pool<OCamera>();

	SolidMedium::me().m_masks[CollisionGroup(CM_ENERGY_0)] = CM_SOLID | CM_GROUND | CM_ENERGY_1 | CM_ENERGY_2;
	SolidMedium::me().m_masks[CollisionGroup(CM_ENERGY_1)] = CM_SOLID | CM_GROUND | CM_ENERGY_0 | CM_ENERGY_2;
	SolidMedium::me().m_masks[CollisionGroup(CM_ENERGY_2)] = CM_SOLID | CM_GROUND | CM_ENERGY_0 | CM_ENERGY_1;

	BlockWorld& world = GlobalPool::me().pool<BlockWorld>().construct("Arcadia");
	game.m_world = &world.m_world;

	//app.m_core->section(0).add_task(game.m_world);

	static Player player = { world };
	game.m_player = Ref(&player);

	world.generate_block(*app.m_visu_system->m_gfx_system, ivec2(0));
	generate_camps(world);
}

void ex_blocks_scene(GameShell& app, GameScene& scene)
{
	static OmniVision vision = { *scene.m_game.m_world };
	
	scene_painters(*scene.m_scene, vision.m_store);
	
	scene.m_scene->painter("World", [&](size_t index, VisuScene& scene, Gnode& parent) {
		UNUSED(scene); paint_scene(parent.subi((void*)index), true);
	});
	scene.m_scene->entity_painter<Shield>("Shields", vision.m_store, paint_shield);
	scene.m_scene->entity_painter<Well>("Well", vision.m_store, paint_well);
	scene.m_scene->entity_painter<Tank>("Tanks", vision.m_store, paint_tank);

	scene.m_scene->entity_painter<TileBlock>("Tileblocks", vision.m_store, paint_block);

	static PhysicDebugDraw physic_draw = { *scene.m_scene->m_scene.m_immediate };

	scene.m_scene->painter("PhysicsDebug", [&](size_t index, VisuScene& visu_scene, Gnode& parent) {
		//physic_draw.draw_physics(parent, *scene.m_game.m_world, SolidMedium::me());
	});
}

void ex_blocks_pump(GameShell& app, Game& game, Widget& parent, Dockbar* dockbar = nullptr)
{
	if(!game.m_player)
	{
		ex_blocks_menu(parent, game);
	}
	else
	{
		ex_blocks_pump_game(app, game, parent);
	}
}

void ex_blocks_pump(GameShell& app, Game& game)
{
#ifdef _BLOCKS_TOOLS
	edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_blocks_pump(app, game, *app.m_editor.m_screen, app.m_editor.m_dockbar);
#else
	ex_blocks_pump(app, game, game.m_screen ? *game.m_screen : app.m_ui->begin());
#endif
}

#ifdef _EX_BLOCKS_EXE
int main(int argc, char *argv[])
{
	cstring example_path = TOY_RESOURCE_PATH "examples/ex_blocks/";
	GameShell app(carray<cstring, 2>{ TOY_RESOURCE_PATH, example_path }, argc, argv);
	
	GameModule module = { _blocks::m(), &ex_blocks_init, &ex_blocks_start, &ex_blocks_pump, &ex_blocks_scene };
	app.run_game(module);
}
#endif
