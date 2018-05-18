//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <visu/VisuScene.h>
#include <block/Generated/Forward.h>
#include <block/Block.h>

#include <gfx/Node3.h>
#include <gfx/Light.h>

/* std */
#include <map>

using namespace mud; namespace toy
{
	void paint_heap(Gnode& parent, Heap& heap);
	void paint_block(Gnode& parent, Block& block);

	struct BlockState : public NodeState
	{
		size_t m_updated = 0;
		std::map<Element*, object_ptr<Model>> m_models;
	};

	void update_block_geometry(GfxSystem& gfx_system, Block& block, BlockState& state);
}
