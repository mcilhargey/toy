//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Generated/Types.h>
#include <editor/Editor/Editor.h>

#include <core/View/Vision.h>
#include <core/Player/Player.h>

#include <visu/VisuScene.h>

#include <lang/VisualScript.h>

using namespace mud; namespace toy
{
	ActionGroup::ActionGroup(const string& name)
		: m_name(name)
	{}

	Editor::Editor(User& user, VisuSystem& visuSystem)
		: m_user(user)
		, m_visuSystem(visuSystem)
		, m_selection(user.m_selection)
		, m_workPlane()//vec3(0.f, 10.f, 0.f), Entity::FrontVector, Entity::RightVector)
		, m_toolbelt()
		, m_actionDeck()
	{
		m_tool_context.m_action_stack = &m_actionDeck;

		m_toolbelt.toolbox("Action").m_tools.push_back(make_object<UndoTool>(m_tool_context));
		m_toolbelt.toolbox("Action").m_tools.push_back(make_object<RedoTool>(m_tool_context));
				  
		//m_toolbelt.toolbox("Entity").m_tools.add(make_object<TranslateTool>(*this));
		//m_toolbelt.toolbox("Entity").m_tools.add(make_object<RotateTool>(*this));
		//m_toolbelt.toolbox("Entity").m_tools.add(make_object<CopyTool>(*this));
				  
		//m_toolbelt.toolbox("View").m_tools.add(top_view_tool(camera));
		//m_toolbelt.toolbox("View").m_tools.add(bottom_view_tool(camera));
		//m_toolbelt.toolbox("View").m_tools.add(front_view_tool(camera));
		//m_toolbelt.toolbox("View").m_tools.add(back_view_tool(camera));
		//m_toolbelt.toolbox("View").m_tools.add(left_view_tool(camera));
		//m_toolbelt.toolbox("View").m_tools.add(right_view_tool(camera));
				  
		//m_toolbelt.toolbox("Brushes").m_tools.add(make_object<PlaceBrush>(*this));
		//m_toolbelt.toolbox("Brushes").m_tools.add(make_object<CircleBrush>(*this));

		//m_toolbox->updateTools();
	}

	Editor::~Editor()
	{}

	void Editor::closeWorld()
	{
		m_scene = nullptr;
		m_editedWorld = nullptr;
		m_editedScene = nullptr;

		//m_graph.clear();

		//m_screen->destroy();
		//m_painterPanel->destroy();
		m_vision = nullptr;
	}

	void Editor::editWorld(World& world)
	{
		//m_graph.setRoot(var(world));

		m_vision = make_object<OmniVision>(world);
		//m_scene = make_object<VisuScene>(*m_vision, m_visuSystem, m_soundManager);

		//m_screen = &m_components.emplace<DVision>(*m_vision);
		//m_painterPanel = &m_editorUi->workspace().emplace<DPainterPanel>(m_scene->visuScene());

		m_editedWorld = &world;
		m_editedScene = m_scene.get();
	}

	void Editor::createScriptedBrush()
	{
#if 0
		Signature signature({ Param("position", var(Zero3)) });
		VisualScript& script = GlobalPool::me().pool<VisualScript>().construct("Brush VisualScript", signature);

		//m_toolbelt.toolbox("Brushes").m_tools.add(make_object<ScriptedBrush>(*this, m_editedWorld->origin(), script));
#endif
	}

	ActionGroup& Editor::actionGroup(const string& name)
	{
		if(m_actionGroups.find(name) == m_actionGroups.end())
			m_actionGroups[name] = make_unique<ActionGroup>(name);

		return *m_actionGroups[name];
	}
}
