//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <visu/Forward.h>
#include <visu/VisuScene.h>

using namespace mud; namespace toy
{
	void build_world_page_geometry(Scene& scene, WorldPage& page);

	void paint_world_page(Gnode& parent, WorldPage& page);
	void paint_navmesh(Gnode& parent, Navmesh& navmesh);
}
