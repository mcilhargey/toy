
#include <platform/ex_platform.h>
#include <platform/Generated/Module.h>
#include <toy/toy.h>

Worldblock::Worldblock(Id id, Entity& parent, const vec3& position, const uvec3& size, const vec3& period, WaveTileset& tileset)
	: Construct(m_entity, proto<Worldblock>())
	, m_entity(id, proto<Worldblock>(), parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_world_page(m_entity, m_emitter, true, vec3(size) * 0.5f)
	, m_navblock(m_entity.m_world.part<Navmesh>(), m_entity, m_world_page)
	, m_tileblock(size, period, tileset)
{
	m_entity.m_world.addTask(this, short(Task::GameObject));
	m_world_page.m_geometry_filter = { "platform/cube_covered", "platform/cube_covered_side", "platform/cube_covered_angle", "platform/corner_covered", "platform/empty_covered" };

	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(*this, m_emitter, m_world_page, m_navblock);
	m_entity.m_parent->m_contents.add(m_entity);

	as<TileWorld>(*m_entity.m_world.m_construct).m_tileblock = this;
}

Worldblock::~Worldblock()
{
	m_entity.m_world.removeTask(this, short(Task::GameObject));
}

void Worldblock::next_frame(size_t frame, size_t delta)
{
	m_tileblock.next_frame(frame, delta);
}

TileWorld::TileWorld(const std::string& name)
	: Construct(m_world, proto<TileWorld>())
	, m_world(0, proto<TileWorld>(), name)
	, m_bulletWorld(m_world)
	, m_navmesh(m_world)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_bulletWorld, m_navmesh, *this);
}

TileWorld::~TileWorld()
{
	m_world.destroy();
}

Human2::Human2(Id id, Entity& parent,
	const vec3& position,
	const std::string& name)
	: Construct(m_entity, proto<Human2>())
	, m_entity(id, proto<Human2>(), parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_name(name)
	, m_solid(m_entity, *this, Capsule(0.35f, 2.f), SolidMedium::me(), CM_SOLID, false, 1.f)
	, m_force(Zero3)
	, m_torque(Zero3)
{
	m_entity.m_world.addTask(this, short(Task::State)); // TASK_GAMEOBJECT

	m_emitter.addSphere(VisualMedium::me(), 0.1f);
	m_receptor.addSphere(VisualMedium::me(), 50.f);

	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_emitter, m_receptor, m_active, *this);
	m_entity.m_parent->m_contents.add(m_entity);
}

Human2::~Human2()
{
	m_entity.m_world.removeTask(this, short(Task::State));
}

void Human2::next_frame(size_t tick, size_t delta)
{
	m_solid.m_impl->setForce(rotate(m_entity.m_rotation, m_force));
	m_solid.m_impl->setTorque(m_torque);
}

void ex_platform_init(GameShell& app, Game& game)
{
	app.m_visu_system->m_gfx_system->create_material("earth", "pbr/pbr", "earth.png", true);
	app.m_visu_system->m_gfx_system->create_material("stone", "debug", "stone.png", true);
}

void ex_platform_start(GameShell& app, Game& game)
{
	game.m_world = &GlobalPool::me().pool<TileWorld>().construct("Arcadia").m_world;

	VisualScript& generator = platform_generator(app);
	generator(std::vector<Var>{ Ref(game.m_world), Ref(&game.m_world->origin()) });
}

void ex_platform_scene(VisuScene& scene)
{
	scene.m_entity_painters.emplace_back(partPainter<Human2, paint_human>("Humans", scene.m_entity_painters.size()));
	scene.m_entity_painters.emplace_back(partPainter<Worldblock, paint_block>("Tileblocks", scene.m_entity_painters.size()));
}

vec3 find_fitting_player_location(Tileblock& tileblock)
{
	vec3 center = vec3(tileblock.m_size) * 0.5f;
	vec3 start_coord = {};
	for(size_t i = 0; i < tileblock.m_tiles.size(); ++i)
		if(tileblock.m_tileset.m_tiles_flip[tileblock.m_tiles[i]].m_name == "cube_covered")
		{
			vec3 coord = { tileblock.m_tiles.x(i), tileblock.m_tiles.y(i), tileblock.m_tiles.z(i) };
			if(distance2(coord, center) < distance2(start_coord, center))
				start_coord = coord;
		}
	return start_coord;
}

void ex_platform_pump(GameShell& app, Game& game, Widget& parent, Dockbar& dockbar)
{
	Worldblock& block = *as<TileWorld>(*game.m_world->m_construct).m_tileblock;

	static bool once = true;
	if(block.m_tileblock.m_wave.m_solved && once)
	{
		once = false;

		block.m_world_page.buildGeometry();

		vec3 start_coord = find_fitting_player_location(block.m_tileblock);
		Entity& character = game.m_world->origin().constructNested<Human2>(start_coord + Y3 * 4.f, "Klaus");
		game.m_units.push_back(&character);
	}

	ex_platform_ui(app.m_ui->begin(), app.m_user, *app.m_visu_system, game);
}

#ifdef _PLATFORM_EXE
void pump(GameShell& app, Game& game)
{
	//edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_platform_pump(app, game, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
}

int main(int argc, char *argv[])
{
	std::string example_path = TOY_RESOURCE_PATH "examples/ex_platform/";
	std::string wfc_path = TOY_RESOURCE_PATH "examples/17_wfc/";
	GameShell app({ TOY_RESOURCE_PATH, example_path, wfc_path }, argc, argv);

	GameModule module = { platform::module(), &ex_platform_init, &ex_platform_start, &pump };
	app.run(module);
}
#endif
