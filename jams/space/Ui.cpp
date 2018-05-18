//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <space/Generated/Types.h>
#include <space/Ui.h>

#include <toy/toy.h>

#include <space/ex_space.h>

#define CAMERA_ROTATION_SPEED 0.03f

static void jump_camera_to(Camera& camera, Entity& target)
{
	animate(&camera, member(&Camera::m_lensDistance), var(random_scalar(1.f, 2.f)), 1.f);
	animate(&camera.m_entity, member(&Entity::m_position), var(target.m_position), 1.f);
	animate(&camera.m_entity, member(&Entity::m_rotation), var(rotate(camera.m_entity.m_rotation, random_scalar(0.f, float(M_PI / 2.f)), Y3)), 1.f);
}

static void move_camera_to(Camera& camera, Entity& target)
{
	animate(&camera.m_entity, member(&Entity::m_position), var(target.m_position), 1.f);
}

Style fleet_sheet_style()
{
	Style style("fleet_sheet", styles().stack, Args{ { &Layout::m_align, Dim2<Align>{ CENTER, CENTER } }, 
														{ &Layout::m_padding, vec4{ 40.f, 10.f, 40.f, 10.f } } });
	style.m_skin.m_empty = false;
	style.m_skin.m_background_colour = Colour::AlphaGrey;
	return style;
}

static Widget& fleet_sheet_widget(Widget& parent, Fleet& fleet)
{
	static Style style = fleet_sheet_style();
	Widget& self = ui::widget(parent, style);

	ui::label(self, "Fleet " + fleet.m_name);
	Widget* spaceships = ui::expandbox(self, { "Spaceships" }, true).m_body;

	if(spaceships)
	{
		Widget& header = ui::row(*spaceships);
		ui::label(header, "Code");
		ui::label(header, "Name");
		ui::label(header, "Number");

		for(auto& kv : fleet.m_composition)
		{
			Widget& row = ui::row(*spaceships);
			ui::label(row, kv.first->m_code);
			ui::label(row, kv.first->m_name);
			ui::label(row, to_string(kv.second));
		}
	}

	return self;
}

static Widget& star_sheet_widget(Widget& parent, Star& star)
{
	static Style style = fleet_sheet_style();
	Widget& self = ui::widget(parent, style);

	ui::label(self, "Star " + star.m_name);
	Widget* ressources = ui::expandbox(self, { "Ressources" }, true).m_body;
	if(ressources)
	{
		ui::label(*ressources, "Minerals : " + to_string(star.m_minerals));
		ui::label(*ressources, "Andrium : " + to_string(star.m_andrium));
	}
	return self;
};

static vec2 galaxy_coord(Viewer& viewer, Galaxy& galaxy)
{
	return galaxy.intersect_coord(viewer.mouse_ray());
}

static bool select_parsec(Widget& parent, Viewer& viewer, Fleet& fleet, vec2& value)
{
	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::Mouse, EventType::Moved))
	{
		fleet.galaxy().m_highlight = galaxy_coord(viewer, fleet.galaxy());
	}

	if(MouseEvent mouse_event = viewer.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
	{
		fleet.galaxy().m_highlight = uvec2{ UINT32_MAX };
		value = galaxy_coord(viewer, fleet.galaxy());
		return true;
	}

	return false;
}

static void jump_action(Fleet& fleet, vec2 coord, FleetStance stance)
{
	fleet.order_jump(coord, stance);
}

struct JumpQuery : public NodeState
{
	JumpQuery() {}
	size_t m_step = 0;
	vec2 m_coord;
	FleetStance m_stance;
};

enum FleetOrderSwitch
{
	FLEET_ORDER_JUMP = 1 << 0
};

static bool jump_query(Widget& parent, Viewer& viewer, Fleet& fleet)
{
	Widget& self = ui::screen(parent);
	JumpQuery& query = self.state<JumpQuery>();
	if(query.m_step == 0 && select_parsec(self, viewer, fleet, query.m_coord))
		query.m_step++;
	if(query.m_step == 1 && select_value<FleetStance>(self, 0, query.m_stance, false))
	{
		fleet.order_jump(query.m_coord, query.m_stance);
		return true;
	}
	return false;
}

static void fleet_orders(Widget& parent, GameState& game, Fleet& fleet)
{
	Widget& self = ui::row(parent);
	if(ui::modal_button(self, self, "Jump", FLEET_ORDER_JUMP))
		jump_query(self, *game.m_viewer, fleet);
}

struct FleetSheetState : public NodeState
{
	Fleet* m_fleet = nullptr;
	vec3 m_position = Zero3;
	Clock m_clock;
};

static void fleet_sheet(Widget& parent, GameState& game, Fleet& fleet)
{
	Widget& self = fleet_sheet_widget(parent, fleet);
	FleetSheetState& state = self.state<FleetSheetState>();

	if(state.m_fleet != &fleet)
	{
		state.m_fleet = &fleet;
		jump_camera_to(game.m_camera, fleet.m_entity);
	}

	if(fleet.m_jump.m_state == FleetJump::END && state.m_position != fleet.m_entity.m_position)
	{
		state.m_position = fleet.m_entity.m_position;
		move_camera_to(game.m_camera, fleet.m_entity);
	}
		
	fleet_orders(parent, game, fleet);
	game.m_camera.m_entity.rotate(Y3, CAMERA_ROTATION_SPEED * float(state.m_clock.step()));
	//object_edit(self, fleet);
}

struct StarSheetState : public NodeState
{
	Star* m_star = nullptr;
	Clock m_clock;
};

static void star_sheet(Widget& parent, GameState& game, Star& star)
{
	Widget& self = ui::sheet(parent);
	StarSheetState& state = self.state<StarSheetState>();

	if(state.m_star != &star)
	{
		state.m_star = &star;
		jump_camera_to(game.m_camera, star.m_entity);
	}

	star_sheet_widget(self, star);
	game.m_camera.m_entity.rotate(Y3, CAMERA_ROTATION_SPEED * float(state.m_clock.step()));
	//member_display(self, &Star::m_minerals);
	//member_display(self, &Star::m_andrium);
	//object_edit(self, star);
}

static void game_item_sheet(Widget& parent, GameState& game, Entity& entity)
{
	if(entity.m_prototype.is<Star>())
		star_sheet(parent, game, entity.part<Star>());
	else if(entity.m_prototype.is<Fleet>())
		fleet_sheet(parent, game, entity.part<Fleet>());
}

static void game_info(Widget& parent, GameState& game)
{
	Widget& self = ui::layout_span(parent, 0.2f);
	if(!game.m_selection.empty())
	{
		Entity& entity = val<Entity>(game.m_selection[0]);
		Widget& sheet = ui::widget(self, styles().sheet, (void*) entity.m_id);
		game_item_sheet(sheet, game, entity);
	}
}

static void game_space(Widget& parent)
{
	Widget& self = ui::layout_span(parent, 0.7f);
}

static void game_actions(Widget& parent)
{
	Widget& self = ui::layout_span(parent, 0.1f);
	if(ui::button(self, "Next Turn").activated())
		printf("Next Turn Pressed !");
}

struct NavigationState : public NodeState
{
	bool m_once = false;
};

static void game_viewport_navigate(Widget& parent, GameState& game)
{
	Universe& universe = as<Universe>(*game.m_game.m_world->m_construct);
	Galaxy& galaxy = *universe.m_galaxies[0];

	Widget& self = ui::sheet(parent);
	NavigationState& state = self.state<NavigationState>();
	if(!state.m_once)
	{
		jump_camera_to(game.m_camera, galaxy.m_stars[0]->m_entity);
		state.m_once = true;
	}

	game_info(self, game);
	game_space(self);
	game_actions(self);
}

void ex_space_ui(Widget& parent, toy::User& user, VisuSystem& visuSystem, Game& game)
{
	Widget& self = ui::board(parent);
	GameState& state = self.state<GameState>(user, visuSystem, game);

	auto custom_modes = [&](Widget& parent) {
		//parent.mode_decl(EDIT_MODE_GAME, game_viewport_navigate, state);
	};
	game_screen(self, state, custom_modes);
}
