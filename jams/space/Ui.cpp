//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <space/Types.h>
#include <space/Ui.h>
#include <space/Scene.h>

#include <meta/math/Convert.h>
#include <meta/space/Convert.h>

#include <toy/toy.h>

#include <space/ex_space.h>

#define CAMERA_ROTATION_SPEED 0.03f

void label_entry(Widget& parent, cstring name, cstring value)
{
	Widget& row = ui::table_row(parent); ui::label(row, name); ui::label(row, value);
}

template <class T>
void number_entry(Widget& parent, cstring name, T value)
{
	Widget& row = ui::table_row(parent); ui::label(row, name); ui::label(row, to_string(value).c_str());
}

void commander_emblem(Widget& parent, Commander& commander)
{
	Widget& row = ui::row(parent);
	ui::image256(row, commander.m_name.c_str(), commander.m_avatar, vec2(50.f));
	ui::title(row, commander.m_name.c_str());
}

Style& sheet_style()
{
	static Colour light_grey = Colour::DarkGrey;
	static Colour dark_grey = Colour(0.07f, 0.07f, 0.07f, 0.5f);
	static Style style("SpaceSheet", styles().wedge, [](Layout& l) { l.m_space = { PARAGRAPH, SHRINK, WRAP }; l.m_align = { CENTER, CENTER }; l.m_padding = vec4(15.f); l.m_spacing = vec2(10.f); },
													 [](InkStyle& s) { s.m_empty = false; s.m_background_colour = dark_grey; s.m_border_colour = light_grey; s.m_border_width = vec4(1.f); });
	return style;
}

Style& order_style()
{
	static Style style("OrderLabel", styles().label, {}, [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::Red; });
	return style;
}

Style& event_style()
{
	static Style style("EventLabel", styles().label, {}, [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::Red; });
	return style;
}

bool select_parsec(Widget& parent, Viewer& viewer, Fleet& fleet, vec2& value)
{
	UNUSED(parent);
	Widget& screen = ui::overlay(viewer);

	if(MouseEvent mouse_event = screen.mouse_event(DeviceType::Mouse, EventType::Moved))
	{
		fleet.galaxy().m_highlight = fleet.galaxy().intersect_coord(viewer.mouse_ray());
	}

	if(MouseEvent mouse_event = screen.mouse_event(DeviceType::MouseLeft, EventType::Stroked))
	{
		fleet.galaxy().m_highlight = uvec2{ UINT32_MAX };
		value = fleet.galaxy().intersect_coord(viewer.mouse_ray());
		return true;
	}

	return false;
}

void jump_action(Fleet& fleet, vec2 coord, FleetStance stance)
{
	fleet.order_jump(coord, stance);
}

struct JumpQuery : public NodeState
{
	JumpQuery() {}
	size_t m_step = 0;
	vec2 m_coord = vec2(0.f);
	FleetStance m_stance = FleetStance::Movement;
};

struct SplitQuery : public NodeState
{
	SplitQuery() {}
	std::string m_name;
	std::map<ShipSchema*, size_t> m_ships;
	FleetStance m_stance = FleetStance::Movement;
};

struct BuildQuery : public NodeState
{
	BuildQuery() {}
	Schema* m_schema = nullptr;
	int m_number = 0;
	Fleet* m_destination = nullptr;
};

enum FleetOrderSwitch
{
	FLEET_ORDER_JUMP = 1 << 0,
	FLEET_ORDER_SPLIT = 1 << 1,
};

void jump_query(Widget& parent, Viewer& viewer, Fleet& fleet)
{
	Widget& self = ui::screen(parent);
	JumpQuery& query = self.state<JumpQuery>();
	if(query.m_step == 0 && select_parsec(self, viewer, fleet, query.m_coord))
		query.m_step++;
	if(query.m_step == 1 && select_value<FleetStance>(self, 0, query.m_stance, false))
	{
		fleet.order_jump(query.m_coord, query.m_stance);
		parent.m_switch &= ~FLEET_ORDER_JUMP;
	}
}

void split_query(Widget& parent, Fleet& fleet)
{
	Widget& self = ui::auto_modal(parent, FLEET_ORDER_SPLIT);
	Widget& sheet = ui::widget(self, sheet_style());

	SplitQuery& query = self.state<SplitQuery>();

	ui::label(sheet, ("Split Fleet " + fleet.m_name).c_str());

	ui::input_field<std::string>(sheet, "Name", query.m_name);
	ui::enum_field<FleetStance>(sheet, "Directive", query.m_stance);

	Table& table = ui::table(sheet, carray<cstring, 4>{ "Code", "Name", "Number", "Split" }, carray<float, 4>{ 0.2f, 0.6f, 0.2f, 0.2f });

	for(auto& kv : fleet.m_ships)
	{
		Widget& row = ui::table_row(table);
		ui::label(row, kv.first->m_code.c_str());
		ui::label(row, kv.first->m_name.c_str());
		ui::label(row, to_string(kv.second).c_str());
		ui::number_input<size_t>(row, { query.m_ships[kv.first], StatDef<size_t>{ 0, fleet.m_ships[kv.first] } });
	}

	if(ui::button(sheet, "Split").activated())
	{
		fleet.order_split(query.m_name.c_str(), query.m_stance, query.m_ships);
		parent.m_switch &= ~FLEET_ORDER_SPLIT;
	}
}

void fleet_orders(Widget& parent, Viewer& viewer, Fleet& fleet)
{
	Widget& self = ui::row(parent);
	if(ui::modal_button(self, self, "Jump", FLEET_ORDER_JUMP))
		jump_query(self, viewer, fleet);
	if(ui::modal_button(self, self, "Split", FLEET_ORDER_SPLIT))
		split_query(self, fleet);

	if(fleet.m_jump.m_state == FleetJump::Ordered)
		ui::item(parent, order_style(), ("Jump to [" + to_string(fleet.m_jump.m_destination) + "]").c_str());
	if(fleet.m_split.m_state == FleetSplit::Ordered)
		ui::item(parent, order_style(), ("Split to " + fleet.m_split.m_name).c_str());
}

void fleet_summary(Widget& parent, Fleet& fleet)
{
	Table& table = ui::columns(parent, carray<float, 2>{ 0.6f, 0.4f });

	label_entry(table, "Code", fleet.m_name.c_str());
	number_entry(table, "Speed", fleet.m_speed);
	number_entry(table, "Scan", fleet.m_scan);
	number_entry(table, "Spatial Power", fleet.m_spatial_power);
	number_entry(table, "Planetary Power", fleet.m_planetary_power);
	label_entry(table, "Size", to_string(fleet_size(fleet.m_spatial_power)).c_str());
}

void fleet_scan_sheet(Widget& parent, Fleet& fleet)
{
	Widget& self = ui::widget(parent, sheet_style());

	commander_emblem(self, *fleet.m_commander);

	Table& table = ui::columns(self, carray<float, 2>{ 0.6f, 0.4f });

	label_entry(table, "Code", fleet.m_name.c_str());
	label_entry(table, "Size", to_string(fleet_size(fleet.m_spatial_power)).c_str());
	label_entry(table, "Experience", to_string(fleet_experience(fleet.m_experience)).c_str());
}

void fleet_sheet(Widget& parent, Viewer& viewer, Fleet& fleet)
{
	Widget& self = ui::widget(parent, sheet_style());

	commander_emblem(self, *fleet.m_commander);

	ui::label(self, ("Fleet " + fleet.m_name).c_str());
	
	fleet_summary(self, fleet);

	Widget* spaceships = ui::expandbox(self, "Spaceships", true).m_body;

	if(spaceships)
	{
		Table& table = ui::table(*spaceships, carray<cstring, 3>{ "Code", "Name", "Number" }, carray<float, 3>{ 0.2f, 0.6f, 0.2f });

		for(auto& kv : fleet.m_ships)
		{
			Widget& row = ui::table_row(table);
			ui::label(row, kv.first->m_code.c_str());
			ui::label(row, kv.first->m_name.c_str());
			ui::label(row, to_string(kv.second).c_str());
		}
	}

	ui::label(self, "Orders");

	fleet_orders(self, viewer, fleet);
}

Widget& schema_row(Widget& parent, Schema& schema, bool selected)
{
	Widget& row = ui::table_row(parent);
	ui::label(row, schema.m_code.c_str());
	ui::label(row, schema.m_name.c_str());
	ui::label(row, "Building");
	ui::label(row, to_string(schema.m_level).c_str());
	Widget& cost = ui::stack(row);
	ui::label(cost, ("Centaure : " + to_string(schema.m_cost)).c_str());
	ui::label(cost, ("Minerals : " + to_string(schema.m_minerals)).c_str());
	ui::label(row, "");
	row.set_state(SELECTED, selected);
	return row;
}

void launch_build_sheet(Widget& parent, Star& star, uint32_t mode)
{
	Widget& self = ui::auto_modal(parent, mode, { 800.f, 600.f });
	Widget& sheet = ui::scroll_sheet(*self.m_body);

	Table& table = ui::table(*sheet.m_body, carray<cstring, 6>{ "Code", "Name", "Kind", "Level", "Cost", "Stats" }, 
											carray<float, 6>  {  0.15f,  0.3f,   0.1f,   0.05f,   0.15f,  0.25f  });

	BuildQuery& query = self.state<BuildQuery>();

	for(BuildingSchema& schema : BuildingDatabase::me().m_schemas)
	{
		if(schema_row(table, schema, query.m_schema == &schema).activated())
			query.m_schema = &schema;
	}

	for(ShipSchema& schema : ShipDatabase::me().m_schemas)
	{
		if(schema_row(table, schema, query.m_schema == &schema).activated())
			query.m_schema = &schema;
	}

	ui::number_field<int>(self, "Number", { query.m_number, { 0, 99'999 } });

	if(ui::button(self, "Submit").activated())
	{
		star.add_construction(*query.m_schema, query.m_number, query.m_destination);
		parent.m_switch &= ~mode;
	}
}

void star_scan_sheet(Widget& parent, Star& star)
{
	Widget& self = ui::widget(parent, sheet_style());

	if(star.m_commander)
		commander_emblem(self, *star.m_commander);

	Table& table = ui::columns(self, carray<float, 2>{ 0.6f, 0.4f });

	label_entry(table, "Code", star.m_name.c_str());
	label_entry(table, "Population", (to_string(star.m_population) + "/" + to_string(star.m_max_population)).c_str());
	number_entry(table, "Environment", star.m_environment);
}

void star_sheet(Widget& parent, Star& star)
{
	Widget& self = ui::widget(parent, sheet_style());

	if(star.m_commander)
		commander_emblem(self, *star.m_commander);

	ui::label(self, ("Star " + star.m_name).c_str());

	Widget& info = ui::row(self);
	label_entry(info, "Population", (to_string(star.m_population) + "/" + to_string(star.m_max_population)).c_str());
	number_entry(info, "Environment", star.m_environment);

	Widget* resources = ui::expandbox(self, "Resources", true).m_body;
	if(resources)
	{
		Table& table = ui::columns(*resources, carray<float, 2>{ 1.f, 1.f});
		for(Resource resource = Resource(0); resource != Resource::Count; resource = Resource(size_t(resource) + 1))
			if(star.m_resources[size_t(resource)] > 0)
			{
				number_entry(table, to_string(resource).c_str(), star.m_resources[size_t(resource)]);
			}
	}

	Widget* stocks = ui::expandbox(self, "Stocks", true).m_body;
	if(stocks)
	{
		Table& table = ui::columns(*stocks, carray<float, 2>{ 1.f, 1.f});
		for(Resource resource = Resource(0); resource != Resource::Count; resource = Resource(size_t(resource) + 1))
			if(star.m_stocks[size_t(resource)] > 0)
			{
				number_entry(table, to_string(resource).c_str(), star.m_stocks[size_t(resource)]);
			}
	}

	Widget* buildings = ui::expandbox(self, "Buildings", true).m_body;
	if(buildings)
	{
		Table& table = ui::columns(*buildings, carray<float, 2>{ 1.f, 1.f});
		for(auto& kv : star.m_buildings)
			number_entry(table, kv.first->m_name.c_str(), kv.second);
	}

	Widget* constructing = ui::expandbox(self, "Pending Constructions", true).m_body;
	if(constructing)
	{
		Table& table = ui::table(*constructing, carray<cstring, 3>{ "Code", "Number", "ETA" }, carray<float, 3>{ 1.f, 1.f, 1.f });
		for(Construction& construction : star.m_constructions)
		{
			Widget& row = ui::table_row(table);
			ui::label(row, construction.m_schema->m_code.c_str());
			ui::label(row, to_string(construction.m_number).c_str());
			ui::label(row, to_string(construction.m_turns).c_str());
		}
	}

	Widget* actions = ui::expandbox(self, "Actions", true).m_body;
	if(actions)
	{
		enum Modes { Build = (1 << 0) };
		if(ui::modal_button(*actions, *actions, "Start Construction", Build))
			launch_build_sheet(*actions, star, Build);
		ui::button(*actions, "Change Taxation");
		ui::button(*actions, "Change Politics");
		ui::button(*actions, "Transfer System");
	}
}

Widget& sheet(Widget& parent, cstring title)
{
	Widget& self = ui::widget(parent, sheet_style());
	ui::title(self, title);
	return self;
}

void commander_sheet(Widget& parent, Commander& commander)
{
	Widget& self = sheet(parent, "Commander");

	commander_emblem(self, commander);

	Table& table = ui::columns(self, carray<float, 2>{ 1.f, 1.f});
	number_entry(table, "Command", commander.m_command);
	number_entry(table, "Commerce", commander.m_commerce);
	number_entry(table, "Diplomacy", commander.m_diplomacy);
}

void empire_sheet(Widget& parent, Commander& commander)
{
	Widget& self = sheet(parent, "Government");

	Table& statistics = ui::columns(self, carray<float, 2>{ 1.f, 1.f});
	label_entry(statistics, "Regime", to_string(commander.m_regime).c_str());
	number_entry(statistics, "Systems", commander.m_stars.size());
	number_entry(statistics, "Fleets", commander.m_fleets.size());
	number_entry(statistics, "Centaures", commander.m_centaures);
	number_entry(statistics, "Power", commander.m_power);
}

void technology_sheet(Widget& parent, Commander& commander)
{
	Widget& self = sheet(parent, "Technology");

	Table& technology = ui::table(self, carray<cstring, 4>("Name", "Level", "Points", "Budget"), carray<float, 4>{ 0.45f, 0.15f, 0.2f, 0.2f });
	for(size_t i = 0; i != size_t(Technology::Count); ++i)
	{
		TechDomain& techno = commander.m_technology[i];
		Widget& row = ui::table_row(technology);
		ui::label(row, to_string(Technology(i)).c_str());
		ui::label(row, to_string(techno.m_level).c_str());
		ui::label(row, to_string(techno.m_points).c_str());
		ui::number_input<float>(row, { techno.m_budget, StatDef<float>{} });
	}
}

void turn_report_stage(Widget& parent, Turn& turn, Commander& commander, TurnStage stage)
{
	Widget& self = sheet(parent, "Events");
	for(const TurnEvents::Item& item : turn.m_events[&commander].m_items[size_t(stage)])
	{
		ui::item(self, event_style(), item.m_summary.c_str());
	}
}

void turn_report_divisions(Widget& parent, Galaxy& galaxy, Turn& turn)
{
	UNUSED(parent); UNUSED(galaxy);
	turn.next_stage();
}

void jump_camera_to(toy::Camera& camera, const vec3& target, const quat& rotation, float distance, float angle, float duration = 1.f)
{
	animate(Ref(&camera), member(&toy::Camera::m_lens_distance), var(distance), duration);
	animate(Ref(&camera), member(&toy::Camera::m_lens_angle), var(angle), duration);
	animate(Ref(&as<Transform>(camera.m_entity)), member(&Transform::m_position), var(target), duration);
	animate(Ref(&as<Transform>(camera.m_entity)), member(&Transform::m_rotation), var(rotation), duration);
}

void turn_report_movements(Widget& parent, GameScene& game, Turn& turn)
{
	sheet(parent, "Movements");

	Fleet* jumping = turn.current_jump();
	
	if(jumping)
	{
		jumping->m_jump.update(*jumping, jumping->m_entity.m_last_tick);
		if(jumping->m_jump.m_state_updated == jumping->m_entity.m_last_tick)
		{
			vec3 position = to_xz(vec2(jumping->m_coord)) + 0.5f + Y3;
			vec3 destination = to_xz(vec2(jumping->m_jump.m_destination)) + 0.5f + Y3;
			quat rotation = look_at(position, destination);

			if(jumping->m_jump.m_state == FleetJump::Start)
				jump_camera_to(game.m_camera, position, rotation, 2.f, c_pi / 8.f, 3.f);
			else if(jumping->m_jump.m_state == FleetJump::Jump)
				jump_camera_to(game.m_camera, destination, 2.f);
		}
		if(jumping->m_jump.m_state == FleetJump::None)
			turn.m_current_jump++;
	}

	if(!jumping)
		turn.next_stage();
}

void fleet_losses_sheet(Widget& parent, const CombatFleet& combat_fleet, float t)
{
	Fleet& fleet = *combat_fleet.m_fleet;
	ui::label(parent, ("Fleet " + fleet.m_name + " of commander " + fleet.m_commander->m_name).c_str());

	Table& table = ui::table(parent, carray<cstring, 3>{ "Code", "Name", "Losses" }, carray<float, 3>{ 0.2f, 0.6f, 0.2f });

	for(auto& kv : fleet.m_ships)
	{
		ShipSchema* ship = kv.first;
		uint32_t number = kv.second;
		uint32_t destroyed = uint32_t(float(combat_fleet.m_losses.at(ship)) * t);

		Widget& row = ui::table_row(table);
		ui::label(row, kv.first->m_code.c_str());
		ui::label(row, kv.first->m_name.c_str());
		ui::label(row, (to_string(destroyed) + "/" + to_string(number)).c_str());
	}
}

void combat_report_sheet(Widget& parent, SpatialCombat& combat)
{
	Widget& self = sheet(parent, "Combat");

	ui::label(self, "Allies");
	for(const CombatFleet& combat_fleet : combat.m_allies)
		fleet_losses_sheet(self, combat_fleet, combat.m_t_damage);

	ui::label(self, "Enemies");
	for(const CombatFleet& combat_fleet : combat.m_enemies)
		fleet_losses_sheet(self, combat_fleet, combat.m_t_damage);
}

void turn_report_combats(Widget& parent, GameScene& game, Turn& turn)
{
	sheet(parent, "Combats");

	SpatialCombat* combat = turn.current_combat();

	if(combat)
	{
		//vec3 position = to_xz(vec2(turn.m_current_combat->m_coord)) + 0.5f + Y3;
		//jump_camera_to(game.m_camera, position);

		combat_report_sheet(parent, *combat);
		if(ui::button(parent, "Next").activated())
		{
			combat->apply_losses();
			turn.m_current_combat++;
		}
	}
	else
	{
		turn.next_stage();
	}
}

void turn_report_constructions(Widget& parent, Player& player, Turn& turn)
{
	sheet(parent, "Constructions");
	turn_report_stage(parent, turn, *player.m_commander, TurnStage::Constructions);
}

void turn_report_sheet(Widget& parent, GameScene& game, Player& player, Turn& turn, Commander& commander)
{
	UNUSED(commander);
	sheet(parent, "Turn Report");

	if(turn.m_stage == TurnStage::None)
		turn.next_stage();
	else if(turn.m_stage == TurnStage::Divisions)
		turn_report_divisions(parent, *player.m_galaxy, turn);
	else if(turn.m_stage == TurnStage::Movements)
		turn_report_movements(parent, game, turn);
	else if(turn.m_stage == TurnStage::Combats)
		turn_report_combats(parent, game, turn);
	else if(turn.m_stage == TurnStage::Systems)
		turn_report_stage(parent, turn, *player.m_commander, TurnStage::Systems);
	else if(turn.m_stage == TurnStage::Constructions)
		turn_report_constructions(parent, player, turn);
	//else if(turn.m_state == TurnStage::Revolts)
	//	turn_report_divisions(parent, *player.m_galaxy, turn);
	else if(turn.m_stage == TurnStage::Technology)
		turn_report_stage(parent, turn, *player.m_commander, TurnStage::Technology);
	//else if(turn.m_state == TurnStage::Payments)
	//	turn_report_divisions(parent, *player.m_galaxy, turn);

}

template <class T>
inline T* try_value(Ref object) { if(type(object).template is<T>()) return &val<T>(object); else return nullptr; }

void space_item_sheet(Widget& parent, Viewer& viewer, Player& player, Ref object)
{
	Star* star = try_value<Star>(object);
	Fleet* fleet = try_value<Fleet>(object);
	if(star != nullptr)
	{
		if(star->m_commander == player.m_commander)
			star_sheet(parent, *star);
		else
			star_scan_sheet(parent, *star);
	}
	else if(fleet != nullptr)
	{
		if(fleet->m_commander == player.m_commander)
			fleet_sheet(parent, viewer, *fleet);
		else
			fleet_scan_sheet(parent, *fleet);
	}
}

static void game_actions(Widget& parent, Player& player)
{
	Widget& self = sheet(parent, "Actions");

	if(ui::button(self, "Next Turn").activated())
	{
		player.m_last_turn = { player.m_galaxy->m_commanders };
		player.m_turn_replay = { player.m_galaxy->m_commanders };
		//player.m_turn_replay = player.m_last_turn;
		player.m_mode = GameMode::TurnReport;
	}
}

Widget& division(Widget& parent, float span)
{
	static Style style("GameDivision", styles().layout, [](Layout& l) { l.m_padding = vec4(40.f); l.m_spacing = vec2(40.f); });

	Widget& self = ui::widget(parent, style);
	self.m_frame.set_span(DIM_X, span);
	self.m_frame.set_span(DIM_Y, span);
	return self;
}

void shrink_switch(Widget& parent, array<cstring> labels, size_t& value)
{
	static Style style("ShrinkSwitch", styles().wedge, [](Layout& l) { l.m_space = { READING, SHRINK, SHRINK }; l.m_align = { CENTER, CENTER }; });

	Widget& middlebox = ui::widget(parent, style);
	ui::radio_switch(middlebox, labels, value);
}

static void game_viewer_ui(Widget& parent, GameScene& game, Player& player)
{
	Widget& self = ui::screen(parent);

	Widget& header = ui::header(self);
	shrink_switch(header, carray<cstring, 3>{ "Empire", "Tactics", "Turn Report" }, (size_t&) player.m_mode);
	
	Widget& board = ui::board(self);
	Widget& left = division(board, 0.3f);
	Widget& middle = division(board, 0.45f);
	Widget& right = division(board, 0.25f);
	UNUSED(middle);

	if(player.m_mode == GameMode::Empire)
	{
		enum Modes : size_t { Overview, Technology };
		static Modes mode = Overview;
		shrink_switch(left, carray<cstring, 2>{ "Overview", "Technology" }, (size_t&) mode);

		if(mode == Overview)
		{
			commander_sheet(left, *player.m_commander);
			empire_sheet(left, *player.m_commander);
		}
		else if(mode == Technology)
		{
			technology_sheet(left, *player.m_commander);
		}

		player.m_selected_item = {};

		static Ref hovered = player.m_hovered_item;
		static Clock clock;
		if(hovered != player.m_hovered_item)
		{
			hovered = player.m_hovered_item;
			clock.step();
		}
		if(hovered && clock.read() > 0.2f)
		{
			space_item_sheet(right, *game.m_viewer, player, hovered);
		}
	}
	else if(player.m_mode == GameMode::Tactics)
	{
		Ref selected = game.m_selection.size() > 0 ? game.m_selection[0] : Ref();
		
		if(selected)
		{
			Widget& sheet = ui::widget(left, styles().sheet, selected.m_value);
			space_item_sheet(sheet, *game.m_viewer, player, selected);
		}

		game_actions(right, player);

		if(player.m_selected_item != selected)
		{
			Entity& entity = type(selected).is<Fleet>() ? val<Fleet>(selected).m_entity : val<Star>(selected).m_entity;
			jump_camera_to(game.m_camera, entity.m_position, random_scalar(1.f, 2.f), random_scalar(float(-c_pi / 8.f), float(c_pi / 8.f)));
			player.m_selected_item = selected;
		}

		static Clock clock;
		game.m_camera.m_entity.rotate(Y3, CAMERA_ROTATION_SPEED * float(clock.step()));
	}
	else if(player.m_mode == GameMode::TurnReport)
	{
		turn_report_sheet(left, game, player, player.m_turn_replay, *player.m_commander);
	}
}

void ex_space_ui(Widget& parent, GameScene& game)
{
	Player& player = val<Player>(game.m_game.m_player);

	Widget& self = ui::widget(parent, styles().board, &game);

	Viewer& viewer = game_viewport(self, game);
	paint_viewer(viewer);

	game_viewer_ui(viewer, game, player);

	player.m_hovered_item = viewer.m_hovered;

#if 0
	if(fleet.m_jump.m_state == FleetJump::END && state.m_position != fleet.m_entity.m_position)
	{
		state.m_position = fleet.m_entity.m_position;
		move_camera_to(game.m_camera, fleet.m_entity);
	}
#endif
}
