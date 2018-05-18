//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/NonCopy.h>
#include <obj/Unique.h>
#include <obj/Util/Updatable.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>

/* std */
#include <map>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT SubPhysicWorld
	{
	public:
		SubPhysicWorld(World& world, Medium& medium);
		virtual ~SubPhysicWorld() {}

		World& m_world;
		Medium& m_medium;

		virtual void updateContacts() = 0;
		virtual void next_frame(size_t tick, size_t delta) = 0;

		virtual object_ptr<ColliderImpl> makeCollider(Collider& collider) = 0;
		virtual object_ptr<ColliderImpl> makeSolid(Solid& solid) = 0;

		virtual void addSolid(Solid& solid) = 0;
		virtual void removeSolid(Solid& solid) = 0;

		virtual void addCollider(Collider& collider) = 0;
		virtual void removeCollider(Collider& collider) = 0;
	};

	class _refl_ TOY_CORE_EXPORT PhysicWorld : public NonCopy, public Updatable
    {
	public:
        PhysicWorld(World& world);
		~PhysicWorld();

		_attr_ World& m_world;

		void next_frame(size_t tick, size_t delta);

		SubPhysicWorld& getSubWorld(Medium& medium);

	public:
		virtual object_ptr<SubPhysicWorld> createSubWorld(Medium& medium) = 0;
		virtual vec3 groundPoint(const Ray& ray) = 0;

	protected:
		std::map<Medium*, object_ptr<SubPhysicWorld>> m_subWorlds;
    };
}
