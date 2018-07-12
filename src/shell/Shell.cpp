
#include <shell/Shell.h>

#include <toy/toy.h>
#include <toy/Modules.h>

#include <ui-vg/VgVg.h>

#include <edit/Edit/Viewport.h>

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

	GameScene& Game::add_scene()
	{
		m_scenes.emplace_back(make_unique<GameScene>(*m_user, *m_shell->m_visu_system, *this, m_player));
		return *m_scenes.back();
	}

	GameScene::GameScene(User& user, VisuSystem& visu_system, Game& game, Ref player)
		: m_scene(make_object<VisuScene>(visu_system))
		, m_selection(user.m_selection)
		, m_camera(game.m_world->origin().construct<OCamera>(vec3(10.f, 0.f, 10.f), 25.f, 0.1f, 300.f).m_camera)
		//, m_camera(m_vision->addCamera(vec3(0.f, 0.f, -25.f), 25.f, true))
		, m_game(game)
		, m_player(player)
	{
		m_scene->m_player = player;
		m_scene->m_scene.m_user = player;
		m_camera.setLensAngle(c_pi / 4.f);
	}

	GameScene::~GameScene()
	{}

	Viewer& game_viewport(Widget& parent, GameScene& game)
	{
		Viewer& self = scene_viewport(parent, *game.m_scene, game.m_camera, game.m_selection);
		game.m_viewer = &self;
		return self;
	}

	GameShell::GameShell(array<cstring> resource_paths, int argc, char *argv[])
		: m_exec_path(exec_path(argc, argv))
		, m_resource_path(resource_paths[0])
		, m_core(make_object<Core>())
		, m_interpreter()
		, m_visu_system(make_object<VisuSystem>(resource_paths))
		, m_editor(*m_visu_system)
		, m_game(m_user, *m_visu_system->m_gfx_system)
	{
		System::instance().load_modules({ &mud_obj::m(), &mud_math::m(), &mud_geom::m(), &mud_lang::m(), &toy_util::m(), &toy_core::m() });
		System::instance().load_modules({ &mud_ui::m(), &mud_gfx::m(), &mud_gfx_pbr::m(), &mud_gfx_obj::m(), &mud_gfx_gltf::m(), &mud_gfx_ui::m() });

		//toyvisu::m().set_visu_module(*m_visu_system);

		System::instance().load_modules({ &toy_visu::m() });
		System::instance().load_modules({ &toy_block::m(), &mud_procgen::m(), &mud_tool::m() });
		//this->connectDb("database.sqlite");

		// @todo this should be automatically done by math module
		register_math_conversions();

		m_interpreter = make_object<LuaInterpreter>();

		m_game.m_shell = this;
		m_game.m_editor.m_script_editor.m_interpreter = m_interpreter.get();

		this->init();
	}

	GameShell::~GameShell()
    {}


#if 0
	void GameShell::launch_game()
	{
		string shell_path = m_exec_path + "/" + "shell";
		System::instance().launch_process(shell_path, m_game_name);
	}
#endif

	void GameShell::launch(const string& module_path)//name, bool start)
	{
		this->load(module_path);

		//if(m_game_module)
		//	this->loadGame(*m_game_module, start);
	}

	void GameShell::init()
	{
		m_visu_system->init();

		m_context = m_visu_system->m_gfx_system->create_context("mud EditorCore", 1600, 900, false);
		//m_context = m_gfx_system.create_context("mud EditorCore", 1280, 720, false);
		GfxContext& context = as<GfxContext>(*m_context);

#if defined MUD_VG_VG
		m_vg = make_object<VgVg>(m_resource_path.c_str(), &m_visu_system->m_gfx_system->m_allocator);
#elif defined MUD_VG_NANOVG
		m_vg = make_object<VgNanoBgfx>(m_resource_path.c_str());
#endif
		context.m_reset_vg = [&] { return m_vg->load_texture(context.m_target->m_diffuse.idx); };

		m_ui_window = make_unique<UiWindow>(*m_context, *m_vg);

		pipeline_pbr(*m_visu_system->m_gfx_system, *m_visu_system->m_gfx_system->m_pipeline);
		m_visu_system->m_gfx_system->init_pipeline();

		static ImporterOBJ obj_importer(*m_visu_system->m_gfx_system);
		static ImporterGltf gltf_importer(*m_visu_system->m_gfx_system);

		//string stylesheet = "minimal.yml";
		string stylesheet = "vector.yml";
		//string stylesheet = "blendish_dark.yml";
		set_style_sheet(*m_ui_window->m_styler, (string(m_ui_window->m_resource_path) + "interface/styles/" + stylesheet).c_str());

		declare_gfx_edit();

		m_ui = m_ui_window->m_root_sheet.get();
	}

	void GameShell::load(GameModule& game_module)
	{
		m_game_module = &game_module;
		m_game_module->m_on_init(*this, m_game);

		System::instance().load_module(*game_module.m_module);

		//if(m_game_module->m_on_start)
		//	m_game_module->m_on_start(*this, m_game);
		//
		//m_core->section(0).add_task(m_game.m_world);
		//
		//m_game_module->m_on_pump(*this, m_game);
	}

	void GameShell::load(const string& module_name)
	{
		Module* module = system().open_module((m_exec_path + "/" + module_name).c_str());
		if(module == nullptr)
		{
			printf("ERROR: could not locate/load module %s\n", module_name.c_str());
			return;
		}

		GameCallback init  = (GameCallback)module_function(*module, (module_name + "_init").c_str());
		GameCallback start = (GameCallback)module_function(*module, (module_name + "_start").c_str());
		GameCallback pump  = (GameCallback)module_function(*module, (module_name + "_pump").c_str());
		SceneCallback scene = (SceneCallback)module_function(*module, (module_name + "_scene").c_str());

		m_game_module_alloc = make_unique<GameModule>(*module, init, start, pump, scene);

		this->load(*m_game_module_alloc);
	}

	void GameShell::run(size_t iterations)
	{
#ifdef MUD_PLATFORM_EMSCRIPTEN
		g_app = this;
		//g_iterations = iterations;
		emscripten_set_main_loop(iterate, 0, 1);
#else
		size_t frame = 0;
		while(pump() && (iterations == 0 || frame++ < iterations));
#endif
	}

	void GameShell::run_game(GameModule& module, size_t iterations)
	{
		this->load(module);
		m_pump = [&](GameShell& app, Game& game) { m_game_module->m_on_pump(app, game); };
		this->run(iterations);
	}

	void GameShell::run_editor(GameModule& module, size_t iterations)
	{
		this->load(module);
		m_pump = [&](GameShell& app, Game& game) { this->pump_editor(); };
		this->run(iterations);
	}

	void GameShell::run_game(const string& module_name, size_t iterations)
	{
		this->load(module_name);
		m_pump = [&](GameShell& app, Game& game) { m_game_module->m_on_pump(app, game); };
		this->run(iterations);
	}

	void GameShell::run_editor(const string& module_path, size_t iterations)
	{
		this->load(module_path);
		m_pump = [&](GameShell& app, Game& game) { this->pump_editor(); };
		this->run(iterations);
	}

	bool GameShell::pump()
	{
		bool pursue = m_ui_window->input_frame();
		m_core->next_frame();
		m_pump(*this, m_game);
		m_ui_window->render_frame();
		pursue &= m_visu_system->next_frame();
		return pursue;
	}

	void GameShell::pump_editor()
	{
		Widget& ui = m_ui->begin();
		toy::editor(ui, m_editor);

		m_game.m_screen = m_editor.m_screen;
		m_editor.m_edited_world = m_game.m_world;

		if(!m_game.m_world)
		{
			m_game_module->m_on_start(*this, m_game);
			m_game_module->m_on_pump(*this, m_game);
			return;
		}

		if(m_editor.m_run_game)
			m_game_module->m_on_pump(*this, m_game);
		else if(m_game.m_scenes.size() > 0)
			game_viewport(*m_game.m_screen, *m_game.m_scenes[0]);
	}

	GameScene& GameShell::add_scene()
	{
		GameScene& scene = m_game.add_scene();
		m_game_module->m_on_scene(*this, scene);
		return scene;
	}

	void GameShell::save()
	{
		//GlobalLoader::me().getLoader(type<World>()).save(m_game.m_world);
		//GlobalLoader::me().getLoader(type<Entity>()).save(&m_game.m_world->origin());
	}

	World& GameShell::loadWorld(Id id)
	{
		//if(GlobalLoader::me().getLoader(type<World>()).dataLoader().check(id, "id", var(id)))
		Ref ref = GlobalLoader::me().getLoader(type<World>()).load(id);
		//World& world = as<World>(val<Complex>(ref));
		World& world = val<World>(ref);
		GlobalLoader::me().getLoader(type<Entity>()).fill(Ref(&world.origin()), world.m_id);
		return world;
	}

	void GameShell::destroyWorld()
	{
		Complex& complex = m_game.m_world->m_complex;
		GlobalPool::me().pool(complex.m_prototype.m_type).destroy(Ref(&complex));
		m_core->section(0).remove_task(m_game.m_world);
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
