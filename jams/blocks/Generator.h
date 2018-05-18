
#pragma once

#include <blocks/Generated/Forward.h>

using namespace mud;
using namespace toy;

_EX_BLOCKS_EXPORT _func_ void paint_block_height(Block& block, Image256& image);
_EX_BLOCKS_EXPORT _func_ void paint_block_elements(Block& block, Image256& image);

void block_generator(GameShell& shell, VisualScript& script);
VisualScript& block_generator(GameShell& shell);

