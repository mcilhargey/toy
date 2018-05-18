//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <obj/Util/Updatable.h>
#include <geom/Shape/ProcShape.h>
#include <core/Generated/Forward.h>
#include <core/Navmesh/rcTileMesh.h>
#include <core/Navmesh/NavGeom.h>

using namespace mud; namespace toy
{
	TOY_CORE_EXPORT uint32_t navmesh_num_vertices(const Navmesh& navmesh);
	
	TOY_CORE_EXPORT ShapeSize size_shape_lines(const ProcShape& shape, const NavmeshShape& navmesh);
	TOY_CORE_EXPORT void draw_shape_lines(const ProcShape& shape, const NavmeshShape& navmesh, MeshData& data);

	TOY_CORE_EXPORT ShapeSize size_shape_triangles(const ProcShape& shape, const NavmeshShape& navmesh);
	TOY_CORE_EXPORT void draw_shape_triangles(const ProcShape& shape, const NavmeshShape& navmesh, MeshData& data);

	class _refl_ TOY_CORE_EXPORT Navmesh : public rcTileMesh, public Updatable
    {
    public:
		_constr_ Navmesh(World& world);
		~Navmesh();

		_attr_ World& m_world;
		_attr_ size_t m_updated = 0;
		_attr_ bool m_dirty = false;

		void update_block(Navblock& navblock);

		virtual void next_frame(size_t tick, size_t delta) final;

		void load();
		void build();

		void save(const char* path);
		void load(const char* path);

	private:
		dtNavMesh* _load(const char* path);
		void _save(const char* path, const dtNavMesh* mesh);
    };

	class _refl_ TOY_CORE_EXPORT Navblock : public Updatable
	{
	public:
		_constr_ Navblock(Navmesh& navmesh, Entity& entity, WorldPage& worldPage);
		~Navblock();

		_attr_ Entity& m_entity;
		_attr_ WorldPage& m_worldPage;
		_attr_ Navmesh& m_navmesh;
		_attr_ size_t m_updated = 0;

		virtual void next_frame(size_t tick, size_t delta) final;
	};

	class _refl_ TOY_CORE_EXPORT NavmeshShape : public Shape
	{
	public:
		_constr_ NavmeshShape(Navmesh& navmesh);

		Navmesh& m_navmesh;

		virtual object_ptr<Shape> clone() const;
	};
}
