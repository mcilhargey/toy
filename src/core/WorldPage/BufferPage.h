//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Store/Array.h>
#include <math/Vec.h>
#include <core/Generated/Forward.h>
#include <core/Physic/Medium.h>
#include <core/Entity/Entity.h>

/* std */
#include <memory>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT BufferPage : public StoreObserver<Entity>
    {
	public:
		_constr_ BufferPage(Entity& entity, WorldPage& worldPage, bool loaded = false);
        ~BufferPage();

		_attr_ Entity& m_entity;
		_attr_ WorldPage& m_worldPage;
		_attr_ bool m_loaded;

		void handleAdd(Entity& entity);
		void handleRemove(Entity& entity);

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
