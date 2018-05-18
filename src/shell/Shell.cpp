
#include <shell/Shell.h>

#include <mud/mud.h>
#include <toy/toy.h>

#if MUD_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

using namespace mud; namespace toy
{
#if MUD_PLATFORM_EMSCRIPTEN
	static GameShell* g_app = nullptr;
	static void iterate()
	{
		g_app->pump();
	}
#endif

	string exec_path(int argc, char *argv[])
	{
#ifdef _WIN32
		UNUSED(argc);
		string exec_path = argv[0];
		string exec_dir(exec_path.begin(), exec_path.begin() + exec_path.rfind('\\'));
#else
		UNUSED(argc); UNUSED(argv);
		string exec_dir = "./";
#endif
		return exec_dir;
	}

	Game::Game(User& user, GfxSystem& gfx_system)
		: m_user(&user)
		, m_editor(gfx_system)
	{}

	Game::~Game()
	{}

	GameState::GameState(User& user, VisuSystem& visuSystem, Game& game)
		: m_vision(make_object<OmniVision>(*game.m_world))
		, m_scene(make_object<VisuScene>(*m_vision, visuSystem))
		, m_selection(user.m_selection)
		, m_camera(m_vision->addCamera(vec3(10.f, 0.f, 10.f), 25.f, true))
		//, m_camera(m_vision->addCamera(vec3(0.f, 0.f, -25.f), 25.f, true))
		, m_game(game)
	{
		m_camera.setLensAngle(M_PI / 4.f);
		m_scene->m_scene_state = this;
	}

	GameState::~GameState()
	{}

	void game_viewport(Widget& parent, GameState& game)
	{
		Viewer& self = scene_viewport(parent, game.m_scene->m_scene, game.m_gfx_camera, game.m_selection);
		game.m_viewer = &self;
	}

	void game_screen(Widget& parent, GameState& game)
	{
		Widget& self = ui::layout(parent);
		game_viewport(self, game);
		//console_view(parent, game);
	}

	GameShell::GameShell(array<cstring> resource_paths, int argc, char *argv[])
		: m_exec_path(exec_path(argc, argv))
		, m_resource_path(resource_paths[0])
		, m_core(make_object<Core>())
		, m_interpreter()
		, m_visu_system(make_object<VisuSystem>(resource_paths))
		, m_editor(*m_visu_system->m_gfx_system)
		, m_game(m_user, *m_visu_system->m_gfx_system)
	{
		System::instance().load_modules({ &mudobj::module(), &mudmath::module(), &mudgeom::module(), &mudlang::module(), &toyutil::module(), &toycore::module() });
		System::instance().load_modules({ &mudui::module(), &mudgfx::module() });

		//toyvisu::module().set_visu_module(*m_visu_system);

		System::instance().load_modules({ &toyvisu::module() });
		System::instance().load_modules({ &toyblock::module(), &mudgen::module(), &mudedit::module() });
		//this->connectDb("database.sqlite");

		m_interpreter = make_object<LuaInterpreter>();

		m_game.m_shell = this;
		m_game.m_editor.m_script_editor.m_interpreter = m_interpreter.get();

		this->init();
	}

	GameShell::~GameShell()
    {}


#if 0
	void GameShell::launchGame()
	{
		string shellPath = m_exec_path + "/" + "shell";
		System::instance().launchProcess(shellPath, m_game_name);
	}
#endif

	void GameShell::launch(const string& module_path)//name, bool start)
	{
		UNUSED(module_path);
		//m_game_name = name;
		//m_game_path = m_exec_path + "/" + name;
		//m_game_module = &as<GameModule>(*System::instance().openModule(name));
		////m_game_module = &as<GameModule>(*System::instance().loadModule(m_game_path));
		//
		//if(m_game_module)
		//	this->loadGame(*m_game_module, start);
	}

	void GameShell::init()
	{
		m_visu_system->init();

		m_ui_window = &m_visu_system->m_gfx_system->create_window("toy EditorCore", 1600, 900, false, nullptr);//&m_user);
		//m_ui_window = &m_visuSystem->m_gfx_system->create_window("toy EditorCore", 1280, 720, false, &m_user);

		//string stylesheet = "minimal.yml";
		string stylesheet = "vector.yml";
		//string stylesheet = "blendish_dark.yml";
		set_style_sheet(*m_ui_window->m_styler, (string(m_ui_window->m_resource_path) + "interface/styles/" + stylesheet).c_str());

		declare_gfx_edit();

		m_ui = m_ui_window->m_root_sheet.get();
	}

	void GameShell::run(GameModule& game_module, size_t iterations)
	{
		m_game_module = &game_module;
		m_game_module->m_on_init(*this, m_game);

		System::instance().load_module(*game_module.m_module);

		if(m_game_module->m_on_start)
			m_game_module->m_on_start(*this, m_game);
		m_core->section(0).addTask(m_game.m_world);

#ifdef MUD_PLATFORM_EMSCRIPTEN
		g_app = this;
		//g_iterations = iterations;
		emscripten_set_main_loop(iterate, 0, 1);
#else
		size_t frame = 0;
		while(pump() && (iterations == 0 || frame++ < iterations));
#endif
	}

	bool GameShell::pump()
	{
		m_game_module->m_on_pump(*this, m_game);
		m_core->next_frame();
		Animator::me().next_frame(0, 0);
		return m_visu_system->next_frame();
	}

	void GameShell::save()
	{
		//GlobalLoader::me().getLoader(type<World>()).save(m_game.m_world);
		//GlobalLoader::me().getLoader(type<Entity>()).save(&m_game.m_world->origin());
	}

	World& GameShell::loadWorld(Id id)
	{
		//if(GlobalLoader::me().getLoader(type<World>()).dataLoader().check(id, "id", var(id)))
		World& world = as<World>(val<Construct>(GlobalLoader::me().getLoader(type<World>()).load(id)).m_stem);
		GlobalLoader::me().getLoader(type<Entity>()).fill(&world.origin(), world.m_id);
		return world;
	}

	void GameShell::destroyWorld()
	{
		Construct& construct = *m_game.m_world->m_construct;
		GlobalPool::me().pool(m_game.m_world->m_prototype).destroy(&construct);
		m_core->section(0).removeTask(m_game.m_world);
		m_game.m_world = nullptr;
	}

	void GameShell::reload()
	{
		Id id = m_game.m_world->m_id;
		this->save();
		this->destroyWorld();

		//GlobalLoader::me().clear();

		m_game_module->m_module = &System::instance().reload_module(*m_game_module->m_module);

		m_game.m_world = &this->loadWorld(id);
		//return *m_game.m_world;
	}

	void GameShell::cleanup()
	{
	}

#ifdef TOY_DB
	void GameShell::connect_db(const string& path)
	{
		m_database = make_object<SqliteDatabase>(path);
		bool success = m_database->init();

		if(!success)
			printf("ERROR: Sqlite Database failed to initialize\n");

		GlobalLoader::me().setDataSource(*m_database);
	}

	void GameShell::reconnect_db(const string& path)
	{
		m_database = nullptr;
		this->connectDb(path);

		// kludge
		GlobalLoader::me().getLoader(type<World>());
		GlobalLoader::me().getLoader(type<Entity>());
	}
#endif
}
