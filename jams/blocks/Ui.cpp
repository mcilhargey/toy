//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <blocks/Ui.h>
#include <toy/toy.h>

void ex_blocks_ui(Widget& parent, toy::User& user, VisuSystem& visuSystem, Game& game)
{
	Widget& self = ui::board(parent);
	GameState& state = self.state<GameState>(user, visuSystem, game);

	auto custom_modes = [](Widget& parent) {};
	game_screen(self, state, custom_modes);
}
