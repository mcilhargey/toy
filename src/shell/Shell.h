
#pragma once

#include <tree/Node.h>
#include <refl/System.h>
#include <shell/Forward.h>
#include <core/Player/Player.h>

#include <edit/Editor/Editor.h>
#include <lang/Lua.h>

#include <visu/VisuModule.h>
#include <visu/VisuScene.h>

#include <uio/Edit/Script.h>

namespace mud
{
	class Shell;
}

using namespace mud; namespace toy
{
	class SqliteDatabase;
	
	using Selection = std::vector<Ref>;

	struct TOY_SHELL_EXPORT GameScene
	{
		object_ptr<VisuScene> m_scene;
		Selection& m_selection;
		Camera& m_camera;
		Game& m_game;
		Ref m_player;
		mud::Camera m_gfx_camera;
		Viewer* m_viewer = nullptr;

		GameScene(User& user, VisuSystem& visuSystem, Game& game, Ref player = {});
		~GameScene();
	};

	struct TOY_SHELL_EXPORT Game : public NonCopy
	{
		Game(User& user, GfxSystem& gfx_system);
		~Game();

		GameScene& add_scene();

		GameShell* m_shell = nullptr;
		User* m_user = nullptr;
		Ref m_player = {};
		World* m_world = nullptr;
		VisualScript* m_generator = nullptr;
		Widget* m_screen = nullptr;
		EditContext m_editor;

		std::vector<unique_ptr<GameScene>> m_scenes;

		std::vector<Entity*> m_units;
	};

	using GameCallback = void(*)(GameShell& shell, Game& game);
	using SceneCallback = void(*)(GameShell& shell, GameScene& scene);

	using EditorPump = std::function<void(GameShell& shell, Game& game)>;

	class TOY_SHELL_EXPORT GameModule
	{
	public:
		GameModule(Module& module, GameCallback on_init, GameCallback on_start, GameCallback on_pump, SceneCallback on_scene)
			: m_module(&module), m_on_init(on_init), m_on_start(on_start), m_on_pump(on_pump), m_on_scene(on_scene)
		{}
		string m_module_path = "";
		Module* m_module = nullptr;
		GameCallback m_on_init = nullptr;
		GameCallback m_on_start = nullptr;
		GameCallback m_on_pump = nullptr;
		SceneCallback m_on_scene = nullptr;
	};

	TOY_SHELL_EXPORT Viewer& game_viewport(Widget& parent, GameScene& game);

	class refl_ TOY_SHELL_EXPORT GameShell : public NonCopy
	{
	public:
		GameShell(array<cstring> resource_paths, int argc, char *argv[]);
		~GameShell();

		void init();
		void launch(const string& module_path);
		void load(GameModule& module);
		void load(const string& module_path);
		void run(size_t iterations = 0U);
		void run_game(GameModule& module, size_t iterations = 0U);
		void run_editor(GameModule& module, size_t iterations = 0U);
		void run_game(const string& module_path, size_t iterations = 0U);
		void run_editor(const string& module_path, size_t iterations = 0U);
		void save();
		void reload();
		bool pump();
		void cleanup();

		void pump_editor();

		GameScene& add_scene();

#ifdef TOY_DB
		void connect_db(const string& path);
		void reconnect_db(const string& path);
#endif

		World& loadWorld(Id id);
		void destroyWorld();

	public:
		string m_exec_path;
		string m_resource_path;

		User m_user;

		object_ptr<Core> m_core;
		object_ptr<LuaInterpreter> m_interpreter;
		object_ptr<VisuSystem> m_visu_system;
		Editor m_editor;

		unique_ptr<Context> m_context;
		unique_ptr<Vg> m_vg;
		unique_ptr<UiWindow> m_ui_window;
		Widget* m_ui = nullptr;

#ifdef TOY_DB
		object_ptr<SqliteDatabase> m_database;
#endif

		unique_ptr<GameModule> m_game_module_alloc;

		GameModule* m_game_module = nullptr;
		Game m_game;

		EditorPump m_pump;
	};
}
