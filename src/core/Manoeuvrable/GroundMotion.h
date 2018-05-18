//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <math/Math.h>
#include <core/Generated/Forward.h>
#include <core/Manoeuvrable/Manoeuvrable.h>
#include <core/Entity/EntityObserver.h>

/* std */
#include <vector>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT GroundMotion : public MotionStrategy, public HookObserver
	{
	public:
		GroundMotion(Entity& entity);

		void hooked();
		void unhooked();
		void adjustHeight(vec3& position);
		void activate(vec3& position);
		void update(const vec3& oldPos, const vec3& pos, vec3& adjustedPos);
		bool validity(const vec3& position);
		void nearestValid(vec3& destination, float margin);

	private:
		Entity& m_entity;
		WorldPage* m_worldPage;
		size_t m_lastUpdated = 0;
		bool m_active = false;
	};
}
