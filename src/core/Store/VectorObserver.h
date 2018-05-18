//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Generated/Forward.h>
#include <obj/Unique.h>
#include <obj/TypeUtils.h>
#include <core/Store/StoreObserver.h>

#include <memory>
#include <vector>

using namespace mud; namespace toy
{
	template <class T, class R>
	struct ResPointer { static inline T* pointer(R& r) { return &r; } };

	template <class T>
	struct ResPointer<T, T*> { static inline T* pointer(T* const& r) { return r; } };

	template <class T>
	struct ResPointer<T, unique_ptr<T>> { static inline T* pointer(const unique_ptr<T>& r) { return r.get(); } };

	template <class R, class T>
	class VectorObserver // : public Observer
	{
	public:
		VectorObserver(std::vector<R>& vector) : m_vector(vector) {}

		void update()
		{
			bool modified = false;

			for(size_t i = 0; i < m_vector.size(); ++i)
			{
				if(i >= m_copy.size())
				{
					for(; i < m_vector.size(); ++i)
						this->handleAdd(*ResPointer<T, R>::pointer(m_vector[i]), i);
					modified = true;
					break;
				}

				if(ResPointer<T, R>::pointer(m_vector[i]) == m_copy[i])
					continue;

				size_t j = i;
				while(j < m_vector.size() - 1 && j < m_copy.size() - 1 && ResPointer<T, R>::pointer(m_vector[i]) != m_copy[j] && ResPointer<T, R>::pointer(m_vector[j]) != m_copy[i])
					++j;

				if(ResPointer<T, R>::pointer(m_vector[i]) == m_copy[j])
					for(; i < j; ++i)
						this->handleAdd(*ResPointer<T, R>::pointer(m_vector[i]), i);
				else if(ResPointer<T, R>::pointer(m_vector[j]) == m_copy[i])
					for(; i < j; ++i)
						this->handleRemove(*m_copy[i], i);
				else
				{
					this->handleAdd(*ResPointer<T, R>::pointer(m_vector[i]), i);
					this->handleRemove(*m_copy[i], i);
				}

				modified = true;
			}

			if(modified)
			{
				m_copy.clear();
				m_copy.resize(m_vector.size());
				for(size_t i = 0; i < m_vector.size(); ++i)
					m_copy[i] = ResPointer<T, R>::pointer(m_vector[i]);
			}
		}

		virtual void handleAdd(T& object, size_t index) = 0;
		virtual void handleRemove(T& object, size_t index) = 0;

	protected:
		std::vector<R>& m_vector;
		std::vector<T*> m_copy;
	};
	
	template <class T_Content, class T_Type = typename type_class<T_Content>::type>
	class GenVectorObserver : public VectorObserver<T_Content, T_Type>
	{
	public:
		GenVectorObserver(std::vector<T_Content>& vector, GenStoreObserver& observer)
			: VectorObserver<T_Content, T_Type>(vector)
			, m_observer(observer)
		{}

		void handleAdd(T_Type& value, size_t index) { UNUSED(index); m_observer.handleAdd(var(value)); }
		void handleRemove(T_Type& value, size_t index) { UNUSED(index); m_observer.handleRemove(var(value)); }

	protected:
		GenStoreObserver& m_observer;
	};
}
