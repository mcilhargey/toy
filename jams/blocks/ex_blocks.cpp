
#include <blocks/ex_blocks.h>
#include <mud/mud.h>
#include <toy/toy.h>

#include <blocks/Generated/Module.h>
#include <shell/Shell.h>

using namespace mud;

Crate::Crate(Id id, Entity& parent, const vec3& position, const vec3& extents)
	: Construct(m_entity, proto<Crate>())
	, m_entity(id, proto<Crate>(), parent, position, ZeroQuat)
	, m_movable(m_entity)
	, m_emitter(m_entity)
	, m_active(m_entity)
	, m_extents(extents)
	, m_solid(m_entity, *this, Cube(extents), SolidMedium::me(), CM_SOLID, false, 1.f)
{
	printf("Crate at %i, %i, %i\n", int(position.x), int(position.y), int(position.z));
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_movable, m_emitter, m_active, *this);
	m_entity.m_parent->m_contents.add(m_entity);
}

BlockWorld::BlockWorld(const std::string& name)
	: Construct(m_world, proto<BlockWorld>())
	, m_world(0, proto<BlockWorld>(), name)
	, m_bulletWorld(m_world)
	, m_navmesh(m_world)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	this->index(m_bulletWorld, m_navmesh, *this);
}

BlockWorld::~BlockWorld()
{
	m_world.destroy();
}

void ex_blocks_init(GameShell& app, Game& game)
{
	app.m_visu_system->m_gfx_system->create_material("crate", "pbr/pbr", true);
	app.m_visu_system->m_gfx_system->create_material("earth", "pbr/pbr", true);
	app.m_visu_system->m_gfx_system->create_material("stone", "pbr/pbr", true);
	//app.m_visu_system->m_gfx_system->create_material("earth", "pbr/pbr", "earth.png", true);
	//app.m_visu_system->m_gfx_system->create_material("stone", "pbr/pbr", "stone.png", true);
}

void ex_blocks_start(GameShell& app, Game& game)
{
	game.m_world = &GlobalPool::me().pool<BlockWorld>().construct("Arcadia").m_world;

	VisualScript& generator = block_generator(app);
	generator(std::vector<Var>{ Ref(game.m_world), Ref(&game.m_world->origin()) });
}

void ex_blocks_scene(VisuScene& scene)
{
	scene.painter("World", [&](size_t index, VisuScene& scene, Gnode& parent) {
		paint_scene(parent.sub((void*)index));
	});

	scene.m_entity_painters.emplace_back(partPainter<Human, paint_human>("Humans", scene.m_entity_painters.size()));
	scene.m_entity_painters.emplace_back(partPainter<Crate, paint_crate>("Crates", scene.m_entity_painters.size()));
}

void ex_blocks_pump(GameShell& app, Game& game, Widget& parent, Dockbar& dockbar)
{
	static bool once = true;
	if(once)
	{
		once = false;

		vec3 position = { 12.5f, 12.5f, 12.5f };
		BlockWorld& app_world = game.m_world->part<BlockWorld>();
		Sector& sector = *game.m_world->part<BlockWorld>().m_sectors[0];
		Entity& character = sector.m_entity.constructNested<Human>(position, 0.35f, 2.f, "Klaus", "Citron");
		game.m_units.push_back(&character);
	}

	ex_blocks_ui(app.m_ui->begin(), app.m_user, *app.m_visu_system, game);
}

#ifdef _BLOCKS_EXE
void pump(GameShell& app, Game& game)
{
	//edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_blocks_pump(app, game, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
}

int main(int argc, char *argv[])
{
	std::string example_path = TOY_RESOURCE_PATH "examples/ex_blocks/";
	GameShell app({ TOY_RESOURCE_PATH, example_path }, argc, argv);
	mudex_05_character::module();

	GameModule module = { blocks::module(), &ex_blocks_init, &ex_blocks_start, &pump };
	app.run(module);
}
#endif
