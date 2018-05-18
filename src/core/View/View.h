//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Store/Array.h>
#include <core/Store/CountStore.h>
#include <core/Generated/Forward.h>
#include <core/Entity/EntityObserver.h>
#include <core/Entity/Entity.h>
#include <core/Physic/Medium.h>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT RecursiveStore : public Array<Entity>, public StoreObserver<Entity>
	{
	public:
		RecursiveStore();

		virtual void handleAdd(Entity& entity);
		virtual void handleRemove(Entity& entity);

		void addRecursive(Entity& entity);
		void removeRecursive(Entity& entity);

	protected:
		std::function<bool(Entity&)> m_filter;
	};

	class _refl_ TOY_CORE_EXPORT View
	{
	public:
		View(Vision& vision, Type& type);
		virtual ~View();

		virtual Store<Entity>& store() = 0;

		_attr_ Type& m_type;
		Vision& m_vision;
	};

	class _refl_ TOY_CORE_EXPORT StoreView : public View
	{
	public:
		StoreView(Vision& vision, const string& name, Store<Entity>& store);

		virtual Store<Entity>& store() { return m_store; }

	protected:
		Store<Entity>& m_store;
	};

    class _refl_ TOY_CORE_EXPORT ReceptorView : public View, public StoreObserver<Entity>
    {
	public:
		ReceptorView(Vision& vision, Medium& medium, Store<Entity>& receptors);
		~ReceptorView();

		Medium& medium() { return m_medium; }

		virtual Store<Entity>& store() { return m_store; }

		void handleAdd(Entity& entity);
		void handleRemove(Entity& entity);

	protected:
		Medium& m_medium;
		Store<Entity>& m_receptors;
		Chained<CountStore<Entity>> m_store;
    };
}
