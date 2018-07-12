//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <edit/Edit/Editor.h>

#include <tool//EditContext.h>
#include <edit/Editor/Editor.h>

#include <infra/StringConvert.h>
#include <obj/Indexer.h>
#include <refl/Meta.h>

#include <edit/Editor/Editor.h>
#include <edit/Editor/Toolbox.h>

#include <core/Entity/Entity.h>
#include <core/World/World.h>
#include <core/View/Vision.h>
#include <core/Selector/Selector.h>

#include <gfx-ui/GfxEdit.h>

#include <uio/Edit/Section.h>
#include <uio/Edit/Injector.h>
#include <uio/Edit/Indexer.h>
#include <uio/Edit/Structure.h>
#include <uio/Edit/Inspector.h>
#include <uio/Object.h>

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
		for(auto& tool : toolbox.m_tools)
			if(ui::button(self, tool->m_name.c_str()).activated())
				tool->activate();
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
			object_item(self, object);
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
		if(!modal_dialog(parent, ("Create " + string(type.m_name)).c_str()))
			object_creator(parent, type);
	}

	void registry_section(Widget& parent, Indexer& indexer, Selection& selection)
	{
		enum Modes { CREATE_OBJECT = 1 << 0 };

		ActionList actions = {
			{ "Create", [&] { parent.m_switch |= CREATE_OBJECT; } }
		};

		Widget& self = section(parent, (string(indexer.m_type.m_name) + " Registry").c_str(), actions);
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

		if(self.m_body)
			for(auto& action : action_group.m_actions)
			{
				if(ui::button(*self.m_body, action.first.c_str()).activated())
					action.second();
			}
	}

	void editor_menu_bar(Widget& parent, Editor& editor)
	{
		Widget& self = ui::menubar(parent);

		for(auto& name_group : editor.m_action_groups)
			editor_menu(self, name_group.second);
	}

	string entity_name(Entity& entity)
	{
		return string(entity.m_complex.m_type.m_name) + ":" + to_string(entity.m_id);
	}

	string entity_icon(Entity& entity)
	{
		return "(" + string(entity.m_complex.m_type.m_name) + ")";
	}

	void outliner_node(Widget& parent, Entity& entity, std::vector<Ref>& selection)
	{
		TreeNode& self = ui::tree_node(parent, carray<cstring, 2>{ entity_icon(entity).c_str(), entity_name(entity).c_str() }, false, false);

		self.set_state(SELECTED, vector_has(selection, Ref(&entity.m_complex)));

		if(self.m_header->activated())
			vector_select(selection, Ref(&entity.m_complex));

		//object_item(self, object);

		if(self.m_body)
			for(Entity* entity : entity.m_contents.store())
			{
				outliner_node(*self.m_body, *entity, selection);
			}
	}

	void outliner_graph(Widget& parent, Entity& entity, std::vector<Ref>& selection)
	{
		ScrollSheet& sheet = ui::scroll_sheet(parent);
		Widget& tree = ui::tree(*sheet.m_body);
		outliner_node(tree, entity, selection);
	}

	void editor_graph(Widget& parent, Editor& editor, Selection& selection)
	{
		Section& self = section(parent, "Outliner");

		if(!editor.m_edited_world)
			return;

		Entity& origin = editor.m_edited_world->origin();
		//structure_view(*self.m_body, Ref(&origin), selection);
		outliner_graph(*self.m_body, origin, selection);
	}

	void editor_components(Widget& parent, Editor& editor)
	{
		static Docksystem docksystem;
		Dockspace& dockspace = ui::dockspace(parent, docksystem);

		std::vector<Type*> library_types = { &type<Entity>(), &type<World>() };
		//if(Widget* dock = ui::dockitem(dockspace, "Library", carray<uint16_t, 1>{ 0U })) //carray<uint16_t, 2>{ 0U, 0U }))
		//	library_section(*dock, library_types, editor.m_selection);
		if(Widget* dock = ui::dockitem(dockspace, "Inspector", carray<uint16_t, 2>{ 0U, 2U })) //carray<uint16_t, 2>{ 0U, 2U }))
			object_editor(*dock, editor.m_selection);
		//edit_selector(self, editor.m_selection); // dockid { 0, 2 }
		if(Widget* dock = ui::dockitem(dockspace, "Graph", carray<uint16_t, 2>{ 0U, 0U })) //carray<uint16_t, 2>{ 0U, 0U }))
			editor_graph(*dock, editor, editor.m_selection);
		//if(Widget* dock = ui::dockitem(dockspace, "Script", carray<uint16_t, 1>{ 1U }))
		//	script_editor(*dock, editor.m_script_editor);
		//current_brush_edit(self, editor); // dockid { 0, 0 }
		//ui_edit(self, editor.m_selection); // dockid { 0, 2 }

		editor.m_screen = ui::dockitem(dockspace, "Screen", carray<uint16_t, 2>{ 0U, 1U });
		
		//if(editor.m_editedScene)
		{
			//scene_viewport(self, *editor.m_editedScene); // dockid { 0, 1 } dockspan 4.f
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
