//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <core/Store/Array.h>
#include <math/Vec.h>
#include <core/Forward.h>
#include <core/Physic/Medium.h>
#include <core/Entity/Entity.h>

#ifndef MUD_CPP_20
#include <memory>
#endif

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT BufferPage : public StoreObserver<Entity>
    {
	public:
		constr_ BufferPage(Entity& entity, WorldPage& world_page, bool loaded = false);
        ~BufferPage();

		attr_ Entity& m_entity;
		attr_ WorldPage& m_world_page;
		attr_ bool m_loaded;

		void handle_add(Entity& entity);
		void handle_remove(Entity& entity);

		void add(Entity& entity);
		void remove(Entity& entity);

		void increment();
		void decrement();

		void load();
		void unload();

		void clear();
		void fill();

    private:
		size_t m_receptors;
    };
}
