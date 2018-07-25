
#include <minimal/ex_minimal.h>
#include <toy/toy.h>

#include <minimal/Api.h>

const vec3 Human::muzzle_offset = { 0.f, 1.6f, -1.f };
float Human::headlight_angle = 40.f;

Human::Human(Id id, Entity& parent, const vec3& position)
	: Complex(id, type<Human>(), m_movable, m_active, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_movable(m_entity)
	, m_active(m_entity)
	, m_walk(false)
	, m_solid(m_entity, *this, CollisionShape(Capsule(0.35f, 1.1f), Y3 * 0.9f), SolidMedium::me(), CM_SOLID, false, 1.f)
{
	m_entity.m_world.add_task(this, short(Task::State)); // TASK_GAMEOBJECT

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
	m_solid->set_angular_factor(Zero3);

	m_visor = this->aim();

	for(auto& bullet : reverse_adapt(m_bullets))
	{
		bullet->update();
		if(bullet->m_destroy)
			vector_remove_pt(m_bullets, *bullet);
	}
}

quat Human::sight(bool aiming)
{
	return aiming ? rotate(m_entity.m_rotation, X3, m_vangle) : m_entity.m_rotation;
}

Aim Human::aim()
{
	quat rotation = this->sight(m_aiming);
	vec3 muzzle = m_entity.m_position + rotate(m_entity.m_rotation, Human::muzzle_offset);
	vec3 direction = rotate(rotation, -Z3);
	vec3 end = muzzle + rotate(rotation, -Z3) * 1000.f;

	Collision hit = m_entity.m_world.part<PhysicWorld>().raycast({ muzzle, end }, CM_GROUND | CM_SOLID);
	return { rotation, muzzle, direction, hit.m_second ? hit.m_hit_point : end, hit.m_second ? &hit.m_second->m_entity : nullptr };
}

void Human::shoot()
{
	Aim aim = this->aim();
	auto fuzz = [](const quat& rotation, const vec3& axis) { return rotate(rotation, axis, random_scalar(-0.05f, 0.05f)); };
	quat rotation = fuzz(fuzz(aim.rotation, X3), Y3);
	m_bullets.emplace_back(make_object<Bullet>(m_entity, aim.start, rotation, 2.f));
	//m_solid->impulse(rotate(m_entity.m_rotation, Z3 * 4.f), Zero3);
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

Player::Player(World& world)
	: m_world(&world)
{}

void Player::spawn(const vec3& position)
{
	m_human = &m_world->origin().construct<Human>(position);
	m_human->m_walk = false;
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

void paint_human(Gnode& parent, Human& human)
{
	static Model& model = human_model_glow(parent.m_scene->m_gfx_system);

	Gnode& self = gfx::node(parent, Ref(&human), human.m_entity.m_position, human.m_entity.m_rotation);
	
	Item& item = gfx::item(self, model, ITEM_SELECTABLE);
	Animated& animated = gfx::animated(self, item);

	if(animated.m_playing.empty() || animated.playing() != human.m_state.name)
		animated.play(human.m_state.name.c_str(), human.m_state.loop, 0.f, human.m_walk ? 0.7f : 1.f);

	Bone* bone = animated.m_rig.m_skeleton.find_bone("RightHand");

	mat4 pose = bxrotation(human.m_entity.m_rotation) * fix_bone_pose(*bone);
	Gnode& arm = gfx::node(self, {}, human.m_entity.m_position + vec3(pose * vec4(Zero3, 1.f)), human.m_entity.m_rotation);
	gfx::model(arm, "rifle");

	enum States { Headlight = 6, Visor = 7 };

	if(human.m_headlight)
	{
		Gnode& headlight = gfx::node(parent.subx(Headlight), Ref(&human), human.m_entity.m_position + rotate(human.m_entity.m_rotation, Human::muzzle_offset), human.sight());
		Light& light = gfx::light(headlight, LightType::Spot, false, Colour::White, 30.f);
		light.m_spot_angle = Human::headlight_angle;
		light.m_spot_attenuation = 0.9f;
	}
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
	Light& light = gfx::directional_light_node(parent);
	
#ifdef TOY_SOUND
	parent.m_sound_manager->threadUpdate();
#endif

	gfx::radiance(parent, "radiance/tiber_1_1k.hdr", BackgroundMode::None);
}

void paint_viewer(Viewer& viewer)
{
	viewer.m_filters.m_glow.m_enabled = true;
	viewer.m_filters.m_glow.m_levels_1_4 = { 1.f, 0.f, 0.f, 0.f };
	viewer.m_filters.m_glow.m_intensity = 0.4f;
#ifndef MUD_MINIMAL_EMSCRIPTEN
	viewer.m_filters.m_glow.m_bicubic_filter = true;
#endif
}

void ex_minimal_scene(GameShell& app, GameScene& scene)
{
	static OmniVision vision = { *scene.m_game.m_world };
	scene.m_camera.m_entity.set_position(Zero3);

	//scene_painters(*scene.m_scene, vision.m_store);
	scene.m_scene->painter("World", [&](size_t index, VisuScene& scene, Gnode& parent) {
		UNUSED(scene); paint_scene(parent.subi((void*)index));
	});
	
	Entity& reference = val<Player>(scene.m_player).m_human->m_entity;

	scene.m_scene->object_painter<Human>("Humans", vision.m_store, paint_human);
	scene.m_scene->object_painter<Crate>("Crates", vision.m_store, paint_crate);
	//scene.m_scene->object_painter<Bullet>("Bullets", vision.m_store, paint_bullet);
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

void human_controller(HumanController& controller, Human& human, OrbitController& orbit, bool relative = true)
{
	vec3 velocity = human.m_solid->linear_velocity();
	vec3 force = relative ? rotate(human.m_entity.m_rotation, controller.m_force)
						  : rotate(quat(vec3(orbit.m_pitch, orbit.m_yaw, 0.f)), controller.m_force);

	human.m_solid->set_linear_velocity({ force.x, velocity.y - 1.f, force.z });
	human.m_solid->set_angular_velocity(controller.m_torque);
}

static void human_velocity_controller(Viewer& viewer, HumanController& controller, Human& human, OrbitController& orbit, bool relative = true)
{
	bool shift = viewer.root_sheet().m_keyboard.m_shift;

	const KeyMove moves[8] =
	{
		{ KC_UP,    -Z3 },{ KC_W, -Z3 },
		{ KC_DOWN,   Z3 },{ KC_S,  Z3 },
		{ KC_LEFT,	-X3 },{ KC_A, -X3 },
		{ KC_RIGHT,  X3 },{ KC_D,  X3 },
	};

	for(const KeyMove& key_move : moves)
		human_control_key(viewer, controller.m_force, controller.m_torque, key_move);

	if(viewer.key_event(KC_SPACE, EventType::Stroked))
		human.m_solid->impulse(Y3 * 16.f, Zero3);

	if(controller.m_force != Zero3 || controller.m_torque != Zero3)
	{
		human.m_state = { human.m_walk ? "Walk" : "RunAim", true };
	}
	else
	{
		human.m_state = { "IdleAim", true };
	}

	human_controller(controller, human, orbit, relative);
}

void ex_minimal_game_hud(Viewer& viewer, GameScene& game, Human& human)
{
	enum Mode { ThirdPerson, Isometric, PseudoIsometric };
	Mode mode = ThirdPerson;

	OrbitController& orbit = mode == Isometric ? ui::isometric_controller(viewer)
											   : ui::orbit_controller(viewer);

	orbit.set_target(human.m_entity.m_position + Y3 * 2.f);

	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::Mouse, EventType::Moved))
	{
		const float rotation_speed = 1.f;
		vec2 angle = -mouse_event.m_delta / 250.f * rotation_speed;

		if(mode != ThirdPerson)
		{
			Ray ray = viewer.m_viewport.ray(mouse_event.m_relative);
			vec3 target = plane_segment_intersection(Plane(Y3, human.m_entity.m_position.y), to_segment(ray));
			if(mode == Isometric)
			{
				human.m_entity.set_rotation(look_at(human.m_entity.m_position, target));
			}
			else if(mode == PseudoIsometric)
			{
				human.m_entity.set_rotation(quat(vec3(0.f, orbit.m_yaw, 0.f)));
				orbit.m_yaw += angle.x;
			}
		}
		else
		{
			human.m_aiming = true;
			human.m_entity.rotate(Y3, angle.x);
			human.m_vangle += angle.y;
			human.m_vangle = min(c_pi / 2.f - 0.01f, max(-c_pi / 2.f + 0.01f, human.m_vangle));
		}
	}

	if(mode == ThirdPerson)
		orbit.set_eye(human.sight());

	if(mode == PseudoIsometric)
	{
		orbit.m_pitch = -c_pi / 4.f;
		orbit.update_eye();
	}

	static HumanController controller;

	human_velocity_controller(viewer, controller, human, orbit, mode != Isometric);

	if(mode == Isometric && controller.m_force != Zero3)
		human.m_entity.set_rotation(look_at(Zero3, rotate(quat(vec3(0.f, orbit.m_yaw, 0.f)), controller.m_force)));

	if(viewer.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
	{
		viewer.take_modal();
		human.shoot();
	}

	if(viewer.key_event(KC_LSHIFT, EventType::Pressed))
		human.m_headlight = !human.m_headlight;
}

void ex_minimal_game_ui(Widget& parent, GameScene& game)
{
	Widget& self = ui::widget(parent, styles().board, &game);//ui::board(parent);

	Viewer& viewer = ui::viewer(self, game.m_scene->m_scene);
	game.m_viewer = &viewer;
	//viewer.take_modal();

	paint_viewer(viewer);

	Player& player = val<Player>(game.m_player);
	if(player.m_human)
		ex_minimal_game_hud(viewer, game, *player.m_human);
}

void ex_minimal_init(GameShell& app, Game& game)
{
	UNUSED(game);
	app.m_visu_system->m_gfx_system->add_resource_path("examples/ex_minimal/");
	app.m_visu_system->m_gfx_system->add_resource_path("examples/05_character/");
	app.m_visu_system->m_gfx_system->add_resource_path("examples/17_wfc/");
}

void ex_minimal_start(GameShell& app, Game& game)
{
	GlobalPool::me().create_pool<TileBlock>();
	GlobalPool::me().create_pool<TileWorld>();
	GlobalPool::me().create_pool<Human>();
	GlobalPool::me().create_pool<Crate>();
	GlobalPool::me().create_pool<OCamera>();

	DefaultWorld& world = GlobalPool::me().pool<DefaultWorld>().construct("Arcadia");
	game.m_world = &world.m_world;

	//app.m_context->lock_mouse(true);

	static Player player = { *game.m_world };
	game.m_player = Ref(&player);
}

void ex_minimal_pump(GameShell& app, Game& game, Widget& parent, Dockbar* dockbar = nullptr)
{
	Player& player = val<Player>(game.m_player);

	game.m_world->next_frame();

	Widget& self = ui::widget(parent, styles().board, &game);

	static GameScene& scene = app.add_scene();
	scene.m_scene->next_frame();

	ex_minimal_game_ui(self, scene);
}

void ex_minimal_pump(GameShell& app, Game& game)
{
	ex_minimal_pump(app, game, game.m_screen ? *game.m_screen : app.m_ui->begin());
}

#ifdef _EX_MINIMAL_EXE
int main(int argc, char *argv[])
{
	cstring example_path = TOY_RESOURCE_PATH "examples/ex_minimal/";
	cstring human_path = TOY_RESOURCE_PATH "examples/05_character/";
	cstring wfc_path = TOY_RESOURCE_PATH "examples/17_wfc/";
	GameShell app(carray<cstring, 4>{ TOY_RESOURCE_PATH, example_path, human_path, wfc_path }, argc, argv);
	
	GameModule module = { _minimal::m(), &ex_minimal_init, &ex_minimal_start, &ex_minimal_pump, &ex_minimal_scene };
	app.run_game(module);
}
#endif
