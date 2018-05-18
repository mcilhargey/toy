
#include <space/ex_space.h>
#include <mud/mud.h>
#include <toy/toy.h>

#include <space/Generated/Convert.h>
#include <space/Generated/Module.h>
#include <shell/Shell.h>

#include <space/Generator.h>
#include <space/Scene.h>
#include <space/Ui.h>

using namespace mud;
using namespace toy;

float g_hullWeight[8] = { 2.f, 6.f, 20.f, 50.f, 100.f, 150.f, 200.f, 250.f };

void fleet_potential(const std::vector<Fleet*>& flotilla, float* damage, float* metal)
{
	for(Fleet* fleet : flotilla)
	{
		for(auto& kv : fleet->m_composition)
		{
			ShipSchema& ship = *kv.first;
			int number = kv.second;

			for(size_t i = 0; i < 8; ++i)
			{
				float modifier = 100.f + fleet->m_commander->m_command
					+ fleet->m_commander->m_technology.m_piloting * 5.f
					+ fleet->m_experience;

				damage[i] += ship.m_spatial.m_values[i] * modifier;
				metal[i] += g_hullWeight[ship.m_size];
			}
		}
	}
}

void resolve_combat(std::vector<Fleet*> first_fleet, std::vector<Fleet*> second_fleet)
{
	float first_damage[8];
	float first_metal[8];

	float second_damage[8];
	float second_metal[8];

	fleet_potential(first_fleet, first_damage, first_metal);
	fleet_potential(second_fleet, second_damage, second_metal);
}

ShipDatabase::ShipDatabase()
{
	m_schemas.emplace_back(ShipSchema{ 1U, "bom", "Bombardier standard", "Galactacorp", 1U,
		19.00f, 25.00f, 0.f,
		6.00f,{}, 1.00f, 8U, 0U });

	m_schemas.emplace_back(ShipSchema{ 1U, "car", "Cargo standard", "Galactacorp", 1U,
		43.00f, 41.00f, 0.f,
		0.00f,{}, 1.00f, 8U, 0U });

	m_schemas.emplace_back(ShipSchema{ 1U, "cha", "Chasseur standard", "Galactacorp", 1U,
		11.00f, 25.00f, 0.f,
		1.00f,{ 10.f, 8.f, 6.f, 4.f, 2.f, 0.f, 0.f, 0.f }, 1.00f, 8U, 0U,{ 0, 2, 0, 0 } });

	m_schemas.emplace_back(ShipSchema{ 1U, "chabom", "Chasseur-bombardier standard", "Galactacorp", 1U,
		15.00f, 25.00f, 0.f,
		3.50f,{ 5.f, 4.f, 3.f, 2.f, 1.f, 0.f, 0.f, 0.f }, 1.00f, 8U, 0U,{ 0, 1, 0, 0 } });

	m_schemas.emplace_back(ShipSchema{ 1U, "colo", "Colonisateur", "Galactacorp", 1U,
		3.00f, 21.00f, 0.f,
		6.00f,{}, 1.00f, 8U, 0U });

	m_schemas.emplace_back(ShipSchema{ 2U, "cor", "Corvette standard", "Galactacorp", 1U,
		87.00f, 69.00f, 0.f,
		4.00f,{ 10.f, 8.f, 9.f, 13.f, 20.f, 24.f, 27.f, 30.f }, 4.00f, 7U, 0U,{ 0, 2, 2, 2 } });

	m_schemas.emplace_back(ShipSchema{ 2U, "fregimp", "Corvette standard", "Galactacorp", 1U,
		369.00f, 104.00f, 0.f,
		16.00f,{ 50.f, 46.f, 50.f, 62.f, 82.f, 52.f, 36.f, 26.f }, 10.00f, 7U, 4U,{ 0, 10, 10, 0 } });

	m_schemas.emplace_back(ShipSchema{ 2U, "sbom", "Super-bombardier standard", "Galactacorp", 1U,
		53.00f, 63.00f, 0.f,
		18.00f,{}, 2.00f, 7U, 0U });

	m_schemas.emplace_back(ShipSchema{ 2U, "sta", "Station spatiale Rakrarior", "n/a", 1U,
		1206.00f, 295.00f, 0.f,
		110.50f,{ 100.f, 103.f, 152.f, 247.f, 388.f, 322.f, 276.f, 253.f }, 22.00f, 7U, 4U });

	m_schemas.emplace_back(ShipSchema{ 3U, "cro", "Croiseur standard", "Galactacorp", 2U,
		408.00f, 194.00f, 0.f,
		13.00f,{ 10.f, 16.f, 32.f, 58.f, 94.f, 64.f, 42.f, 28.f }, 14.00f, 7U, 4U });

	m_schemas.emplace_back(ShipSchema{ 3U, "des", "Destroyer standard", "Galactacorp", 2U,
		308.00f, 192.00f, 0.f,
		11.00f,{ 30.f, 32.f, 43.f, 63.f, 92.f, 56.f, 33.f, 18.f }, 10.00f, 7U, 4U });

	m_schemas.emplace_back(ShipSchema{ 3U, "freg", "Fregate standard", "Galactacorp", 2U,
		296.00f, 192.00f, 0.f,
		11.00f,{ 50.f, 44.f, 43.f, 47.f, 56.f, 32.f, 21.f, 14.f }, 10.00f, 7U, 4U });

	m_schemas.emplace_back(ShipSchema{ 4U, "scro", "Supercroiseur standard", "Galactacorp", 2U,
		766.00f, 461.00f, 0.f,
		58.50f,{ 30.f, 39.f, 69.f, 120.f, 192.f, 138.f, 99.f, 75.f }, 20.00f, 6U, 4U });

	m_schemas.emplace_back(ShipSchema{ 7U, "STA2", "Station Spatiale Rakrarior Evolution", "n/a", 2U,
		1300.00f, 920.00f, 0.f,
		110.50f,{ 310.f, 315.f, 455.f, 731.f, 1142.f, 945.f, 809.f, 742.f }, 29.80f, 11U, 10U });

}

ShipSchema& ShipDatabase::schema(const std::string& code)
{
	for(ShipSchema& schema : m_schemas)
		if(schema.m_code == code)
			return schema;
}

Universe::Universe(const std::string& name)
	: Construct(m_world, proto<Universe>())
	, m_world(0, proto<Universe>(), name)
	, m_bulletWorld(m_world)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_bulletWorld, *this);
}

Universe::~Universe()
{
	m_world.destroy();
}

Galaxy::Galaxy(Id id, Entity& parent, const vec3& position, const uvec2& size)
	: Construct(m_entity, proto<Galaxy>())
	, m_entity(id, proto<Galaxy>(), parent, position, ZeroQuat)
	, m_size(size)
	, m_period{ 1, 1, 1 }
	, m_plane{ Y3, 0.5f }
	, m_highlight(UINT32_MAX)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(*this);
	m_entity.m_parent->m_contents.add(m_entity);

	as<Universe>(*m_entity.m_world.m_construct).m_galaxies.push_back(this);
}

uvec2 Galaxy::intersect_coord(Ray ray)
{
	vec3 intersect = plane_segment_intersection(m_plane, { ray.m_start, ray.m_end });
	uvec3 coord = floor(intersect); // / m_period;
	return{ coord.x, coord.z };
}

Quadrant::Quadrant(Id id, Entity& parent, const vec3& position, const uvec2& coord, float size)
	: Construct(m_entity, proto<Quadrant>())
	, m_entity(id, proto<Quadrant>(), parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_world_page(m_entity, m_emitter, true, vec3(size, size, size))
	, m_buffer_page(m_entity, m_world_page, true)
	, m_coord(coord)
	, m_size(size)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_emitter, m_world_page, m_buffer_page, *this);
	m_entity.m_parent->m_contents.add(m_entity);

	as<Galaxy>(*m_entity.m_parent->m_construct).m_quadrants.push_back(this);
}

static size_t star_count = 0;

Star::Star(Id id, Entity& parent, const vec3& position, const uvec2& coord, const std::string& name)
	: Construct(m_entity, proto<Star>())
	, m_entity(id, proto<Star>(), parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_coord(coord)
	, m_name(name)
{
	m_entity.m_world.addTask(this, short(Task::GameObject));

	m_emitter.addSphere(VisualMedium::me(), 0.1f);
	//m_receptor.addSphere(VisualMedium::me(), 50.f);

	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_emitter, m_receptor, m_active, *this);
	m_entity.m_parent->m_contents.add(m_entity);

	this->galaxy().m_stars.push_back(this);
	//as<Quadrant>(*m_entity.m_parent->m_construct).m_stars.push_back(this);
}

Star::~Star()
{
	m_entity.m_world.removeTask(this, short(Task::GameObject));
}

Galaxy& Star::galaxy() { return m_entity.m_parent->part<Galaxy>(); } // as<Galaxy>(*m_entity.m_parent->m_construct)

void Star::next_frame(size_t tick, size_t delta)
{
}

static size_t fleet_count = 0;

Fleet::Fleet(Id id, Entity& parent, const vec3& position, Commander& commander, const uvec2& coord, const std::string& name)
	: Construct(m_entity, proto<Fleet>())
	, m_entity(id, proto<Fleet>(), parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_commander(&commander)
	, m_coord(coord)
	, m_name(name)
{
	m_entity.m_world.addTask(this, 3); // TASK_GAMEOBJECT

									   // @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_emitter, m_receptor, m_active, *this);
	m_entity.m_parent->m_contents.add(m_entity);

	m_commander->m_fleets.push_back(this);

	this->galaxy().m_fleets.push_back(this);
	this->galaxy().m_grid.add_fleet(*this, coord);
}

Fleet::~Fleet()
{
	m_entity.m_world.removeTask(this, 3);

	vector_remove(m_commander->m_fleets, this);
}

Galaxy& Fleet::galaxy() { return m_entity.m_parent->part<Galaxy>(); } // as<Galaxy>(*m_entity.m_parent->m_construct)

void Fleet::next_frame(size_t tick, size_t delta)
{
	if(m_jump.m_state != FleetJump::DONE)
		m_jump.update(*this, tick);
}

void Fleet::set_ships(ShipSchema& schema, size_t number)
{
	if(number == 0)
		m_composition.erase(&schema);
	else
		m_composition[&schema] = number;

	m_composition_updated = m_entity.m_lastTick;

	this->update_stats();
}

void Fleet::set_ships(const std::string& code, size_t number)
{
	ShipSchema& schema = ShipDatabase::me().schema(code);
	this->set_ships(schema, number);
}

void Fleet::update_stats()
{
	m_spatial = {};
	m_planetary = 0.f;
	m_weapon_ratio = {};
	m_speed = SIZE_MAX;
	m_scan = 0U;

	for(auto& ship_number : m_composition)
	{
		ShipSchema& ship = *ship_number.first;
		float number = ship_number.second;

		m_spatial += ship.m_spatial * number;
		m_planetary += ship.m_planetary * number;
		m_weapon_ratio += ship.m_weapon_ratio * number;
		m_speed = min(m_speed, ship.m_speed);
		m_scan = max(m_scan, ship.m_scan);
	}

	m_weapon_ratio = normalize(m_weapon_ratio);
}

static inline FleetJump::State jump_done(Fleet& fleet, float elapsed) { return FleetJump::DONE; }
static inline FleetJump::State jump_start(Fleet& fleet, float elapsed) { if(elapsed > 0.5f) { fleet.jump(); return FleetJump::JUMP; } else return FleetJump::START; }
static inline FleetJump::State jump_jump(Fleet& fleet, float elapsed) { if(elapsed > 0.25f) return FleetJump::END; else return FleetJump::JUMP; }
static inline FleetJump::State jump_end(Fleet& fleet, float elapsed) { if(elapsed > 0.5f) return FleetJump::DONE; else return FleetJump::END; }

using StateFunc = FleetJump::State(*)(Fleet&, float);
static constexpr StateFunc s_fleet_states[4] = { jump_done, jump_start, jump_jump, jump_end };

void FleetJump::update(Fleet& fleet, size_t tick)
{
	float elapsed = float(tick - m_state_updated) * TICK_INTERVAL;
	State old_state = m_state;
	m_state = s_fleet_states[static_cast<size_t>(m_state)](fleet, elapsed);
	m_state_updated = old_state == m_state ? m_state_updated : tick;
}

void Fleet::order_jump(vec2 coord, FleetStance stance)
{
	printf("Fleet %s from commander %s : jump to [%i,%i] in directive %s\n", m_name.c_str(), m_commander->m_name.c_str(), int(coord.x), int(coord.y), to_string(stance).c_str());
	m_jump = FleetJump{ coord, m_entity.m_lastTick };
	m_stance = stance;
}

void Fleet::jump()
{
	uvec2 coord = m_jump.m_destination;
	this->galaxy().m_grid.move_fleet(*this, m_coord, coord);

	vec3 position = vec3{ coord.x, 0.f, coord.y } +0.5f + Y3;

	if(m_stance == FleetStance::SpatialAttack)
	{
		Flotilla allies;
		Flotilla enemies;

		for(Fleet* fleet : this->galaxy().m_grid.m_fleets[m_coord])
		{
			if(fleet->m_commander->allied(*m_commander))
			{
				fleet->m_entity.m_position = position - Z3 * 0.5f;
				allies.push_back(fleet);
			}
			else
			{
				fleet->m_entity.m_position = position + Z3 * 0.5f;
				enemies.push_back(fleet);
			}
		}

		if(!enemies.empty())
		{
			position = position - Z3 * 0.5f;
			this->galaxy().m_combats.emplace_back(coord, allies, enemies, m_entity.m_lastTick);
		}
	}

	animate(&m_entity, member(&Entity::m_position), var(position), 0.25f);

	//m_entity.setPosition(m_jump.m_destination);
}

void Fleet::order_attack(Star& star)
{
	printf("Fleet %s from commander %s : attack star %s\n", m_name.c_str(), m_commander->m_name.c_str(), star.m_name.c_str());
}

Commander::Commander(Id id, const std::string& name, int command, int commerce, int diplomacy)
	: m_id(index(type<Commander>(), id, this))
	, m_name(name)
	, m_command(command)
	, m_commerce(commerce)
	, m_diplomacy(diplomacy)
{}

Commander::~Commander()
{
	unindex(type<Commander>(), m_id);
}

void Commander::next_frame(size_t tick, size_t delta)
{}

void Commander::take_star(Star& star)
{
	if(star.m_commander)
		vector_remove(star.m_commander->m_stars, &star);
	m_stars.push_back(&star);
	star.m_commander = this;
}

void Commander::take_fleet(Fleet& fleet)
{
	if(fleet.m_commander)
		vector_remove(fleet.m_commander->m_fleets, &fleet);
	m_fleets.push_back(&fleet);
	fleet.m_commander = this;
}

CommanderBrush::CommanderBrush(ToolContext& context)
	: Brush(context, "Commander", type<CommanderBrush>())
	, m_commander(nullptr)
	, m_radius(3.f)
{}

Colour rgb_to_rgba(const Colour& colour, float a)
{
	return{ colour.m_r, colour.m_g, colour.m_b, a };
}

void CommanderBrush::paint(Gnode& parent)
{
	if(!m_commander) return;

	gfx::node(parent, {}, m_position);
	gfx::shape(parent, Circle(m_radius, Axis::Y), Symbol(Colour::White, rgb_to_rgba(m_commander->m_colour, 0.3f)));
}

ToolState CommanderBrush::start()
{
	return SpatialTool::start();
}

void CommanderBrush::update(const vec3& position)
{
	if(!m_commander) return;

	//Galaxy& galaxy = *as<Universe>(*m_context.m_origin->m_world.m_construct).m_galaxies[0];
	//for(Star* star : galaxy.m_stars)
	//	if(distance(star->m_entity.m_position, position) <= m_radius)
	//		m_commander->takeStar(*star);
}

void ex_space_lua_check(GameShell& shell, Galaxy& galaxy)
{
	LuaInterpreter& lua = *shell.m_interpreter;

	lua.set("col", Ref(&Colour::Pink));
	lua.call("print('col.r = ' .. col.r)");
	lua.call("print('col.g = ' .. col.g)");
	lua.call("Colour.Pink.g = 5.93");
	lua.call("print('col.g = ' .. col.g)");
	lua.call("print('Colour.Green.g = ' .. Colour.Green.g)");

	lua.set("galaxy", Ref(&galaxy));
	lua.call("star = galaxy.stars[1]");
	lua.call("fleet = galaxy.fleets[1]");
	lua.call("fleet:order_attack(star)");
	lua.set("coord", var(vec2{ 45.f, 12.f }));
	lua.call("coord = vec2(45, 12)");
	lua.set("stance", var(FleetStance::PlanetaryAttack));
	float x = lua.getx<float>({ "coord", "x" });
	printf("cpp -> coord.x = %f\n", x);
	lua.call("print('coord = ' .. coord.x .. ', ' .. coord.y)");
	lua.call("print('stance = ' .. tostring(stance))");
	lua.call("fleet:order_jump(coord, stance)");

	//lua.call("objects = Index.me.indexer().objects");
}

void ex_space_scene(VisuScene& scene)
{
	scene.m_entity_painters.emplace_back(partPainter<Star, paint_star>("Stars", scene.m_entity_painters.size()));
	scene.m_entity_painters.emplace_back(partPainter<Fleet, paint_fleet>("Fleets", scene.m_entity_painters.size()));
	scene.m_entity_painters.emplace_back(partPainter<Galaxy, paint_galaxy>("Galaxy", scene.m_entity_painters.size()));
}

void ex_space_init(GameShell& app, Game& game)
{
	game.m_editor.m_custom_brushes.emplace_back(make_unique<CommanderBrush>(game.m_editor.m_tool_context));
}

void ex_space_start(GameShell& app, Game& game)
{
	Universe& universe = GlobalPool::me().pool<Universe>().construct("Arcadia");
	game.m_world = &universe.m_world;

	VisualScript& generator = space_generator(app);
	generator(std::vector<Var>{ Ref(game.m_world), Ref(&game.m_world->origin()) });
}

void ex_space_pump(GameShell& app, Game& game, Widget& parent, Dockbar& dockbar)
{
	ex_space_ui(app.m_ui->begin(), app.m_user, *app.m_visu_system, game);
}

#ifdef _SPACE_EXE
void pump(GameShell& app, Game& game)
{
	//edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_space_pump(app, game, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
}

int main(int argc, char *argv[])
{
	std::string example_path = TOY_RESOURCE_PATH "examples/ex_space/";
	GameShell app({ TOY_RESOURCE_PATH, example_path }, argc, argv);

	GameModule module = { space::module(), &ex_space_init, &ex_space_start, &pump };
	app.run(module);
}
#endif
