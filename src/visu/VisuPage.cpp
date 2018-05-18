//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <visu/Generated/Types.h>
#include <visu/VisuPage.h>

#include <core/Movable/Movable.h>
#include <core/Navmesh/Navmesh.h>
#include <core/WorldPage/WorldPage.h>

#include <gfx/Node3.h>
#include <gfx/Item.h>
#include <gfx/Mesh.h>
#include <gfx/GfxSystem.h>

#define DEBUG_NAVMESH_GEOM 0
#define DEBUG_NAVMESH 0

using namespace mud; namespace toy
{
	void paint_world_page(Gnode& parent, WorldPage& page)
	{
		if(!page.m_build_geometry)
			page.m_build_geometry = [&](WorldPage& page) { build_world_page_geometry(*parent.m_scene, page); };
		//gfx::shape(parent, Cube(page.m_extents), Symbol(Colour(1.f, 0.f, 1.f, 0.2f)));
	}

	void paint_navmesh(Gnode& parent, Navmesh& navmesh)
	{
#if DEBUG_NAVMESH_GEOM
		if(navmesh.geom().m_vertices.size())
			gfx::shape(parent, navmesh.geom(), Symbol(Colour::Red, Colour::Red));
#elif DEBUG_NAVMESH
		if(navmesh.m_updated > navmesh.m_worldPage.m_last_rebuilt)
			gfx::shape(parent, NavmeshShape(navmesh), Symbol(Colour::Cyan, Colour::AlphaGrey));
#else
		UNUSED(parent); UNUSED(navmesh);
#endif
	}

	void build_world_page_geometry(Scene& scene, WorldPage& page)
	{
		std::vector<Item*> items;

		VecPool<Item>* pool = scene.m_pool->pool<Item>().m_vec_pool.get();
		for(; pool; pool = pool->m_next.get())
			for(Item* item : pool->m_objects)
			{
				if(!vector_has(page.m_geometry_filter, string(item->m_model->m_name)) || !item->m_node.m_object)
					continue;
				Entity& entity = val<Entity>(item->m_node.m_object);
				if((entity.isChildOf(page.m_entity) || &entity == &page.m_entity) && entity.m_hooked && !entity.isa<Movable>())
					items.push_back(item);
			}

		// @todo : filter on WORLD_GEOMETRY mask ? a way to filter out debug draw geometry ?

		size_t vertexCount = 0;
		size_t indexCount = 0;

		for(Item* item : items)
			for(const ModelItem& model_item : item->m_model->m_items)
			{
				uint16_t num = item->m_instances.empty() ? 1U : uint16_t(item->m_instances.size());
				if(model_item.m_mesh->m_draw_mode != PLAIN)
					continue;
				vertexCount += num * model_item.m_mesh->m_vertexCount;
				indexCount += num * model_item.m_mesh->m_indexCount;
			}

		if(vertexCount == 0 || indexCount == 0)
			return;

		size_t triCount = indexCount / 3;

		Geometry& geom = page.geom();
		geom.allocate(vertexCount, triCount);

		MeshData data(&geom.m_vertices[0], &geom.m_triangles[0].a);

		for(Item* item : items)
			for(const ModelItem& model_item : item->m_model->m_items)
			{
				if(model_item.m_mesh->m_draw_mode != PLAIN)
					continue;

				static mat4 identity = bxidentity();
				if(item->m_instances.empty())
					model_item.m_mesh->read(data, identity);

				for(const mat4& transform : item->m_instances)
					model_item.m_mesh->read(data, transform);
			}

		//m_page.initTerrain(geom);
		//m_page.m_entity.part<Navmesh>().build();

		//worldPage.m_entity.pushPlug(make_object<Pathfinder>(worldPage.m_entity), type<Pathfinder>());
	}
}
