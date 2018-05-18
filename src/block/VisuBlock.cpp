//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <block/VisuBlock.h>

#include <obj/String/StringConvert.h>

#include <block/Block.h>
#include <block/Element.h>
#include <block/Sector.h>
#include <block/Elements.h>

#include <math/Generated/Convert.h>

#include <gfx/Scene.h>
#include <gfx/Item.h>
#include <gfx/GfxSystem.h>

#include <gfx/Draw.h>
#include <gfx/Gfx.h>

#include <geom/Shape/Quad.h>
#include <geom/Shape/Cube.h>

#include <core/Entity/Entity.h>

#define DEBUG_BLOCK 0

using namespace mud; namespace toy
{
	void paint_heap(Gnode& parent, Heap& heap)
	{
		gfx::light(parent, LightType::Point, false, heap.m_element.m_colour, heap.m_radius * 2.f);
		//gfx::shape(parent, Circle(heap.m_radius), Symbol(heap.m_element.m_colour));
		//gfx::shape(parent, Spheroid(heap.radius(), Symbol(heap.m_element.m_colour));
	}

	void paint_block(Gnode& parent, Block& block)
	{
#if DEBUG_BLOCK
		float size = block.m_size / 2.f;
		gfx::shape(parent, Cube(size), Symbol(Colour(1.f, 1.f, 1.f, 0.2f)));
#endif
		BlockState& state = parent.state<BlockState>();
		if(state.m_updated < block.m_updated)
		{
			state.m_updated = block.m_updated;
			update_block_geometry(parent.m_scene->m_gfx_system, block, state);
		}

		for(auto& element_model : state.m_models)
			gfx::item(parent, *element_model.second, ITEM_WORLD_GEOMETRY | ITEM_SELECTABLE);
	}

	void update_block_geometry(GfxSystem& gfx_system, Block& block, BlockState& state)
	{
		if(block.m_subdived)
			return;

		state.m_models.clear();

		std::map<Element*, std::vector<ProcShape>> bodies;

		std::vector<Element*> elements = { &Earth::me(), &Stone::me(), &Sand::me(), &Air::me(), &Gas::me(), &Minerals::me(), &Fungus::me(), &Water::me() };

		size_t index = 0;
		for(Element* element : block.m_chunks)
		{
			if(element == nullptr /*|| element->matterState() != STATE_SOLID*/)
			{
				++index;
				continue;
			}

			for(Side side : Sides)
			{
				Hunk neighbour = block.neighbour(index, side);

				if(!neighbour || neighbour.element == element)
					continue;

				vec3 position = block.chunkPosition(index) + to_vec3(side) * block.chunkSize() / 2.f;

				Colour colour = element->m_colour;
				//Colour colour = Colour::White;
				Quad quad = { block.chunkSize(), NormalX[size_t(side)], NormalY[size_t(side)] };
				bodies[element].push_back({ Symbol(Colour::None, colour), quad, PLAIN, position });
			}

			++index;
		}

		for(Element* element : elements)
			if(!bodies[element].empty())
			{
				string identifier = "sector_" + to_string(block.m_entity.m_id) + "_" + element->m_name;

				printf("INFO: Creating geometry for Block %s, %zu quads\n", identifier.c_str(), bodies[element].size());

				state.m_models[element] = draw_model(gfx_system, identifier.c_str(), bodies[element], true);
				state.m_models[element]->m_meshes[0].m_material = gfx_system.fetch_material(element->m_name.c_str());
				block.sector().m_worldPage.m_geometry_filter.push_back(identifier); // @kludge : find out a more intelligent way to filter world geometry
			}

		//block.sector().m_entity.part<WorldPage>().updateGeometry();
	}
}
