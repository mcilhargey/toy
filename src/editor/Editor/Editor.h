//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <edit/Tool.h>
#include <edit/ActionStack.h>
#include <editor/Generated/Forward.h>
#include <editor/Editor/Toolbox.h>

#include <uio/Edit/Script.h>

using namespace mud; namespace toy
{
	using Selection = std::vector<Ref>;

	class _refl_ TOY_EDITOR_EXPORT ActionGroup
	{
	public:
		ActionGroup(const string& name);

		string m_name;
		std::map<string, std::function<void()>> m_actions;
	};

	class _refl_ TOY_EDITOR_EXPORT Editor : public NonCopy
    {
    public:
		Editor(User& user, VisuSystem& visuSystem);
        ~Editor();

		void closeWorld();
		void editWorld(World& world);

		void createScriptedBrush();

		ActionGroup& actionGroup(const string& name);

	public:
		User& m_user;
		VisuSystem& m_visuSystem;

		Selection& m_selection;
		Plane m_workPlane;

		Toolbelt m_toolbelt;
		ActionStack m_actionDeck;
		ToolContext m_tool_context;
		std::map<string, unique_ptr<ActionGroup>> m_actionGroups;

		World* m_editedWorld;
		VisuScene* m_editedScene;

		object_ptr<Vision> m_vision;
		object_ptr<VisuScene> m_scene;

		ScriptEditor m_scriptEditor;
    };
}
