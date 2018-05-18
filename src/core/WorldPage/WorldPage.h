//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Store/Array.h>
#include <obj/Util/Global.h>
#include <obj/Util/Updatable.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>
#include <core/Physic/Medium.h>
#include <core/Physic/Collider.h>

/* std */
#include <memory>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT WorldMedium : public Medium, public LazyGlobal<WorldMedium>
	{
	public:
		_constr_ WorldMedium();
	};

	/* A WorldPage has : 
		- contents (entities)
		- static geometry (static entities)
		- nested worldpages (worldpages)
		- adjacent worldpages
	*/

	class _refl_ TOY_CORE_EXPORT WorldPage : public NonCopy, public ColliderObject, public Updatable, public StoreObserver<Entity>
    {
	public:
		_constr_ WorldPage(Entity& entity, Emitter& emitter, bool open, const vec3& extents);
        ~WorldPage();

		_attr_ Entity& m_entity;
		_attr_ Emitter& m_emitter;
		_attr_ _mut_ bool m_open;
		_attr_ vec3 m_extents;
		_attr_ World& m_world;
		_attr_ size_t m_last_rebuilt = 0;
		_attr_ size_t m_updated = 0;

		std::vector<string> m_geometry_filter;

		std::function<void(WorldPage&)> m_build_geometry;

		EmitterScope& m_scope;

		Geometry& geom() { return *m_geom; }

		virtual void next_frame(size_t tick, size_t delta) final;

		virtual void addContact(Collider& object);
		virtual void removeContact(Collider& object);

		virtual void handleAdd(Entity& entity);
		virtual void handleRemove(Entity& entity);

		_meth_ void buildGeometry();
		_meth_ void updateGeometry();

		_meth_ void groundPoint(const vec3& position, bool relative, vec3& outputPoint);
		_meth_ void raycastGround(const vec3& from, const vec3& to, vec3& groundPoint);

    private:
		object_ptr<Geometry> m_geom;
		object_ptr<Solid> m_ground;
    };
}
