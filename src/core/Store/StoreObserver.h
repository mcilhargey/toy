//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <core/Generated/Forward.h>
#include <obj/Type.h>

using namespace mud; namespace toy
{
	template <class T>
	class Store;

	class TOY_CORE_EXPORT Observer
	{
	public:
		virtual ~Observer() {}

		virtual void update() {}
	};

	class TOY_CORE_EXPORT GenStoreObserver : public Observer
	{
	public:
		virtual bool handleClear() { return false; }
		virtual void handleAdd(Var value) = 0;
		virtual void handleRemove(Var value) = 0;
	};

	template <class T>
	class StoreObserver : public Observer
	{
	public:
		virtual bool handleClear() { return false; }
		virtual void handleAdd(T& object) = 0;
		virtual void handleRemove(T& object) = 0;
	};

	template <class T>
	class GenericObserver : public StoreObserver<T>
	{
	public:
		GenericObserver(Store<T>& store, GenStoreObserver& observer) : m_store(store), m_observer(observer) { m_store.observe(*this); }
		~GenericObserver() { m_store.unobserve(*this); }

		virtual bool handleClear() { return m_observer.handleClear(); }
		virtual void handleAdd(T& object) { m_observer.handleAdd(var(object)); }
		virtual void handleRemove(T& object) { m_observer.handleRemove(var(object)); }

	protected:
		Store<T>& m_store;
		GenStoreObserver& m_observer;
	};

	template <class T_Object, class T>
	class TypedObserver : public StoreObserver<T_Object>
	{
	public:
		virtual void handleAdd(T_Object& object) { this->handleAdd(as<T>(object)); }
		virtual void handleRemove(T_Object& object) { this->handleRemove(as<T>(object)); }

		virtual void handleAdd(T& object) = 0;
		virtual void handleRemove(T& object) = 0;
	};
}
