
#include <platform/Scene.h>
#include <platform/ex_platform.h>
#include <toy/toy.h>

namespace glm
{
	bool operator<(const uvec3& lhs, const uvec3& rhs)
	{
		return lhs.x < rhs.x ||
			lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y && lhs.z < rhs.z);
	}
}

#define CLEAR_ALL_TILES

void paint_human(Gnode& parent, Human2& human)
{
	//gfx::shape(parent, Cylinder(0.3f, 1.7f), Symbol("human"), ITEM_SELECTABLE);
	gfx::shape(parent, Ellipsis(vec2{ 0.2f, 0.07f }, Axis::Z), Symbol(Colour::White), ITEM_SELECTABLE);
}

void paint_block(Gnode& parent, Worldblock& block)
{
	paint_tileblock(parent, block.m_tileblock);
}

void paint_universe(Gnode& parent, TileWorld& universe)
{
	Light& light = gfx::directional_light_node(parent);

	light.m_shadow_num_splits = 4;
	light.m_shadow_flags = ShadowFlags::CSM_Stabilize;
}