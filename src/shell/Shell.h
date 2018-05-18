
#pragma once

#include <obj/Graph/Node.h>
#include <obj/System/System.h>
#include <shell/Generated/Forward.h>
#include <core/Player/Player.h>

#include <edit/EditContext.h>
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

	struct TOY_SHELL_EXPORT Game : public NonCopy
	{
		Game(User& user, GfxSystem& gfx_system);
		~Game();

		GameShell* m_shell = nullptr;
		User* m_user = nullptr;
		World* m_world = nullptr;
		VisualScript* m_generator = nullptr;
		EditContext m_editor;

		std::vector<Entity*> m_units;
	};

	using GameCallback = std::function<void(GameShell& shell, Game& game)>;

	class TOY_SHELL_EXPORT GameModule
	{
	public:
		GameModule(Module& module, GameCallback on_init, GameCallback on_start, GameCallback on_pump)
			: m_module(&module), m_on_init(on_init), m_on_start(on_start), m_on_pump(on_pump)
		{}
		string m_module_path = "";
		Module* m_module = nullptr;
		GameCallback m_on_init = nullptr;
		GameCallback m_on_start = nullptr;
		GameCallback m_on_pump = nullptr;
	};

	struct TOY_SHELL_EXPORT GameState : public NodeState, public SceneState
	{
		object_ptr<Vision> m_vision;
		object_ptr<VisuScene> m_scene;
		Selection& m_selection;
		Camera& m_camera;
		Game& m_game;
		Camera m_gfx_camera;
		Viewer* m_viewer = nullptr;

		GameState(User& user, VisuSystem& visuSystem, Game& game);
		~GameState();
	};

	TOY_SHELL_EXPORT void game_viewport(Widget& parent, GameState& game);
	TOY_SHELL_EXPORT void game_screen(Widget& parent, GameState& game);

	class _refl_ TOY_SHELL_EXPORT GameShell : public NonCopy
	{
	public:
		GameShell(array<cstring> resource_paths, int argc, char *argv[]);
		~GameShell();

		void init();
		void launch(const string& module_path);
		void run(GameModule& module, size_t iterations = 0U);
		void save();
		void reload();
		bool pump();
		void cleanup();

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
		EditContext m_editor;

		UiWindow* m_ui_window = nullptr;
		Widget* m_ui = nullptr;

#ifdef TOY_DB
		object_ptr<SqliteDatabase> m_database;
#endif

		GameModule* m_game_module = nullptr;
		Game m_game;
	};
}
