//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Store/Array.h>
#include <core/Generated/Forward.h>
#include <core/Entity/EntityObserver.h>

#include <core/Physic/Scope.h>
#include <core/Event/EventTube.h>

/* std */
#include <algorithm>
#include <list>

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT EventEmitter : public Emitter, public EventTube
	{
	public:
		EventEmitter(Entity& entity);
	};

	class _refl_ TOY_CORE_EXPORT EventReceptor : public Receptor, public EventTubeEnd, public StoreObserver<Entity>
	{
	public:
		EventReceptor(Entity& entity);

		void addScope(Medium& medium, float radius, CollisionGroup group = CM_RECEPTOR);
		void removeScope(ReceptorScope& scope);

		void handleAdd(Entity& emitter);
		void handleRemove(Entity& emitter);
	};
}
