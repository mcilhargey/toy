//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Type.h>
#include <obj/Any.h>
#include <obj/Iterable/Iterable.h>
#include <obj/Iterable/Reverse.h>
#include <core/Generated/Forward.h>
#include <core/Store/StoreObserver.h>

#include <algorithm>
#include <vector>

using namespace mud; namespace toy
{
	class GenStore
	{
	public:
		virtual void observe(Observer& observer) = 0;
		virtual void unobserve(Observer& observer, bool notify = false) = 0;
	};

	template <class T>
	class Store : public GenStore
	{
	public:
		virtual void observe(Observer& observer) { this->observe(static_cast<StoreObserver<T>&>(observer)); }
		virtual void unobserve(Observer& observer, bool notify = false) { this->unobserve(static_cast<StoreObserver<T>&>(observer), notify); }

		virtual void observe(StoreObserver<T>& observer) = 0;
		virtual void unobserve(StoreObserver<T>& observer, bool notify = false) = 0;
	};

	template <class T_Array, class T_Element>
	class StoreBase : public Sequence, public Store<T_Element>
	{
	public:
		typedef T_Element T;
		typedef T_Element value_type;

	public:
		StoreBase() {}
		~StoreBase() { this->clear(); }

		inline void select(T& object) { this->clear(); self().add(object); }
		inline void swap(T& object) { self().has(object) ? self().remove(object) : self().add(object); }

		size_t size() const { return self().store().size(); }
		bool empty() const { return self().store().empty(); }

		virtual void add(Ref value) final { self().add(val<T>(value)); }
		virtual void remove(Ref value) final { self().remove(val<T>(value)); }

		virtual void iterate(const std::function<void(Ref)>& callback) const { self().iterate([this, callback](T& object) { return callback(&object); }); }
		virtual bool has(Ref object) const { return self().has(val<T>(object)); }

		std::vector<T*> copy() const
		{
			std::vector<T*> vec;
			for(auto& pt : self().store())
				vec.push_back(&self().deref(pt));
			return vec;
		}

		void iterate(const std::function<void(T&)>& callback) const
		{
			for(const auto& pt : self().store())
				callback(self().deref(pt));
		}

		void clear()
		{
			for(auto observer : reverse_adapt(m_observers))
				if(!observer->handleClear())
					for(auto& pt : self().store())
						observer->handleRemove(self().deref(pt));

			self().store().clear();
		}

		T* find(std::function<bool(T&)> predicate)
		{
			for(auto& pt : self().store())
				if(predicate(self().deref(pt)))
					return &self().deref(pt);
			return nullptr;
		}

		void observe(StoreObserver<T>& observer)
		{
			m_observers.push_back(&observer);
			for(auto& pt : self().store())
				observer.handleAdd(self().deref(pt));
		}

		void unobserve(StoreObserver<T>& observer, bool notify = false)
		{
			vector_remove(m_observers, &observer);
			if(notify)
				for(auto& pt : self().store())
					observer.handleRemove(self().deref(pt));
		}

		void notifyAdd(T& object)
		{
			for(auto& observer : m_observers)
				observer->handleAdd(object);
		}

		void notifyRemove(T& object)
		{
			for(auto& observer : reverse_adapt(m_observers))
				observer->handleRemove(object);
		}

		void resize(size_t size) { UNUSED(size); }

		inline T_Array& self() { return as<T_Array>(*this); }
		inline const T_Array& self() const { return as<T_Array>(*this); }

	protected:
		std::vector<StoreObserver<T>*> m_observers;
	};
}
