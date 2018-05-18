//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <platform/Generated/Types.h>
#include <platform/Ui.h>
#include <platform/ex_platform.h>

#include <toy/toy.h>

void game_item_sheet(Widget& parent, Viewer& viewport, Entity& entity)
{
	//if(entity.m_prototype.is<Star>())
	//	star_sheet(parent, viewport, entity.part<Star>());
}

void game_info(Widget& parent, Viewer& viewport, std::vector<Ref>& selection)
{
	Widget& self = ui::layout_span(parent, 0.2f);
	if(!selection.empty())
		game_item_sheet(parent, viewport, val<Entity>(selection[0]));
}

void game_space(Widget& parent)
{
	Widget& self = ui::layout_span(parent, 0.7f);
}

class HumanController : public NodeState, public EventDispatch
{
public:
	HumanController(Viewer& viewport, Human2& human)
	{
		m_key_down_handlers[KC_Q] = [&] { human.m_torque.y += 1.f; };
		m_key_up_handlers[KC_Q] = [&] { human.m_torque.y -= 1.f; };
		m_key_down_handlers[KC_E] = [&] { human.m_torque.y += -1.f; };
		m_key_up_handlers[KC_E] = [&] { human.m_torque.y -= -1.f; };

		static float force = 1.f;
		m_key_down_handlers[KC_W] = [&] { human.m_force.z += -force; };
		m_key_up_handlers[KC_W] = [&] { human.m_force.z -= -force; };
		m_key_down_handlers[KC_S] = [&] { human.m_force.z += force; };
		m_key_up_handlers[KC_S] = [&] { human.m_force.z -= force; };
		m_key_down_handlers[KC_D] = [&] { human.m_force.x += force; };
		m_key_up_handlers[KC_D] = [&] { human.m_force.x -= force; };
		m_key_down_handlers[KC_A] = [&] { human.m_force.x += -force; };
		m_key_up_handlers[KC_A] = [&] { human.m_force.x -= -force; };
	}
};

void game_actions(Widget& parent, Game& game)
{
	Widget& self = ui::layout_span(parent, 0.1f);
	if(ui::button(self, "Action").activated())
		printf("Action !!");
}

void game_viewport_navigate(Widget& parent, Viewer& viewport, GameState& game)
{
	Widget& self = ui::board(parent);

	if(!game.m_game.m_units.empty())
	{
		HumanController& controller = self.state<HumanController>(viewport, game.m_game.m_units[0]->part<Human2>());
		controller.process(viewport);
	}

	game_info(self, viewport, game.m_selection);
	game_space(self);
	game_actions(self, game.m_game);
}

void ex_platform_ui(Widget& parent, toy::User& user, VisuSystem& visuSystem, Game& game)
{
	Widget& self = ui::board(parent);
	GameState& state = self.state<GameState>(user, visuSystem, game);

	auto custom_modes = [&](Widget& parent) {
		//parent.mode_decl(EDIT_MODE_GAME, tileblock_editor, *state.m_viewer, game);
		//parent.mode_decl(EDIT_MODE_GAME, game_viewport_navigate, *state.m_viewer, state);
	};
	game_screen(self, state, custom_modes);
}
