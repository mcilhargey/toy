//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <math/Math.h>
#include <core/Entity/Entity.h>

using namespace mud; namespace toy
{
	class TOY_CORE_EXPORT MotionSource
	{
	public:
		virtual void updateTransform(const vec3& position, const quat& rotation) = 0;
	};

    class TOY_CORE_EXPORT MotionState
    {
    public:
		MotionState(Entity& entity, MotionSource& source) : m_entity(entity), m_source(source), m_lastUpdated(0) {}
        virtual ~MotionState() {}

		Entity& m_entity;

		void syncTransform(const vec3& position, const quat& rotation)
		{
			if(m_entity.m_lastModified >= m_lastUpdated)
				return;
			m_entity.syncPosition(position);
			m_entity.syncRotation(rotation);
		}

		void update(size_t tick)
		{
			if(m_entity.m_lastModified > m_lastUpdated)
				m_source.updateTransform(m_entity.absolutePosition(), m_entity.absoluteRotation());
			m_lastUpdated = tick;
		}

	protected:
		MotionSource& m_source;
		size_t m_lastUpdated;
    };
}
