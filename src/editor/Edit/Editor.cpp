//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Edit/Editor.h>

#include <edit//EditContext.h>
#include <editor/Editor/Editor.h>

#include <obj/Reflect/Meta.h>
#include <obj/String/String.h>
#include <obj/Indexer.h>

#include <editor/Editor/Editor.h>
#include <editor/Editor/Toolbox.h>

#include <core/Entity/Entity.h>
#include <core/World/World.h>
#include <core/View/Vision.h>
#include <core/Selector/Selector.h>

#include <gfx/Ui/GfxEdit.h>

#include <uio/Edit/Section.h>
#include <uio/Edit/Injector.h>
#include <uio/Edit/Indexer.h>
#include <uio/Edit/Structure.h>
#include <uio/Edit/Inspector.h>
#include <uio/Unode.h>

#include <ui/Structs/Container.h>
#include <ui/Structs/Dock.h>

#include <visu/VisuSystem.h>
#include <visu/VisuScene.h>

using namespace mud; namespace toy
{
	void context_menu(Widget& parent, Selector& selector, Ref object)
	{
		UNUSED(parent); UNUSED(selector); UNUSED(object);
		//popup(parent, [&] { parent.destroy(); }, nullptr);

		//if(selector.m_selection.has(val<IdObject>(object)))
		//	for(auto& method : selector.m_methods.store())
		//		if(ui::button(parent, method->m_name).activated())
		//			selector.execute(*method);
		//else
		//	for(auto& action : selector.m_actions.store())
		//		if(ui::button(parent, action->m_name).activated())
		//			selector.execute(*action);

		//for(auto& action : m_echobject.m_methods)
		//this->emplace<Deck>().maker(&make_device<CarbonMethod, DMethod>).tstore<CarbonMethod>();
	}

	string to_icon(const string& name)
	{
		string clean = replace_all(to_lower(name), " ", "_");
		return "(" + clean + ")";
	}

	void edit_toolbox(Widget& parent, Toolbox& toolbox)
	{
		Widget& self = ui::toolbar(parent);
		UNUSED(toolbox); UNUSED(self);
		/*toolbox.toolstore().iterate([&](Tool& tool) {
			object_hook(parent, Ref(tool));
		});*/
	}

	void edit_toolbelt(Widget& parent, Toolbelt& toolbelt)
	{
		Widget& self = ui::tooldock(parent);

		for(auto& name_toolbox : toolbelt.m_toolboxes)
			edit_toolbox(self, *name_toolbox.second);
	}

	void edit_selection(Widget& parent, Selection& selection)
	{
		Widget& self = ui::select_list(parent);

		for(Ref object : selection)
			object_hook(self, object);
	}

	void edit_selector(Widget& parent, Selector& selector)
	{
		Widget& self = section(parent, "Selector");
		Widget& tabber = ui::tabber(self);

		edit_selection(tabber, selector.m_selection); // "Selection"
		edit_selection(tabber, selector.m_targets); // "Targets"
	}

	void scene_edit(Widget& parent, World& world)
	{
		UNUSED(parent); UNUSED(world);
	}

	void registry(Widget& parent, Indexer& indexer, Selection& selection)
	{
		UNUSED(selection);
		object_indexer(parent, indexer);
	}

	void registry_creator(Widget& parent, Type& type)
	{
		if(!modal_dialog(parent, ("Create " + string(type.m_meta->m_name)).c_str()))
			object_creator(parent, type);
	}

	enum RegistrySwitch
	{
		CREATE_OBJECT = 1 << 0
	};

	void registry_section(Widget& parent, Indexer& indexer, Selection& selection)
	{
		ActionList actions = {
			{ "Create", [&] { parent.m_switch |= CREATE_OBJECT; } }
		};
		Widget& self = section(parent, (string(indexer.m_type.m_meta->m_name) + " Registry").c_str(), actions);
		registry(self, indexer, selection);

		if((parent.m_switch & CREATE_OBJECT) != 0)
			registry_creator(self, indexer.m_type);
	}

	void library(Widget& parent, const std::vector<Type*>& types, Selection& selection)
	{
		Tabber& self = ui::tabber(parent);

		for(Type* type : types)
			if(Widget* tab = ui::tab(self, type->m_name))
			{
				registry_section(*tab, indexer(*type), selection);
			}
	}

	void library_section(Widget& parent, const std::vector<Type*>& types, Selection& selection)
	{
		Widget& self = section(parent, "Library");
		library(self, types, selection);
	}

	void editor_menu(Widget& parent, ActionGroup& action_group)
	{
		Widget& self = ui::menu(parent, action_group.m_name.c_str());

		for(auto& action : action_group.m_actions)
			if(ui::button(self, action.first.c_str()).activated())
				action.second();
	}

	void editor_menu_bar(Widget& parent, Editor& editor)
	{
		Widget& self = ui::menubar(parent);

		for(auto& kv : editor.m_actionGroups)
			editor_menu(self, *kv.second);
	}

	void editor_graph(Widget& parent, Editor& editor, Selection& selection)
	{
		UNUSED(parent); UNUSED(selection);
		if(!editor.m_editedWorld)
			return;

		Entity& origin = editor.m_editedWorld->origin();
		UNUSED(origin);
		//structure_view(parent, &origin, selection);
		// handle selected objects
	}

	void editor_components(Widget& parent, Editor& editor)
	{
		static Docksystem docksystem;
		Dockspace& dockspace = ui::dockspace(parent, docksystem);

		std::vector<Type*> library_types = { &type<Entity>(), &type<World>() };
		if(Widget* dock = ui::dockitem(dockspace, "Library", carray<size_t, 2>{ 0U, 0U }))
			library_section(*dock, library_types, editor.m_selection);
		if(Widget* dock = ui::dockitem(dockspace, "Inspector", carray<size_t, 2>{ 0U, 2U }))
			object_editor(*dock, editor.m_selection);
		//edit_selector(self, editor.m_selection); // dockid { 0, 2 }
		if(Widget* dock = ui::dockitem(dockspace, "Graph", carray<size_t, 2>{ 0U, 0U }))
			editor_graph(*dock, editor, editor.m_selection);
		if(Widget* dock = ui::dockitem(dockspace, "Script", carray<size_t, 2>{ 1U }))
			script_editor(*dock, editor.m_scriptEditor);
		//current_brush_edit(self, editor); // dockid { 0, 0 }
		//ui_edit(self, editor.m_selection); // dockid { 0, 2 }
		
		if(editor.m_editedScene)
		{
			//scene_vision(self, *editor.m_editedScene); // dockid { 0, 1 } dockspan 4.f
			//painter_panel(self, *editor.m_editedScene); // dockid { 0, 2 }
		}
	}

	void editor(Widget& parent, Editor& editor)
	{
		Widget& self = ui::layout(parent);

		editor_menu_bar(self, editor);
		edit_toolbelt(self, editor.m_toolbelt);
		editor_components(self, editor);

		//m_scriptEditor.m_actions.emplace<Response>("Create Scripted Brush", [this] { this->createScriptedBrush(); });
	}
}
