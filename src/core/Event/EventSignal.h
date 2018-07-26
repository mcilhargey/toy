//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <core/Store/Array.h>
#include <core/Forward.h>
#include <core/Entity/EntityObserver.h>

#include <core/Physic/Scope.h>
#include <core/Event/EventTube.h>

#ifndef MUD_CPP_20
#include <algorithm>
#include <list>
#endif

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT EventEmitter : public Emitter, public EventTube
	{
	public:
		EventEmitter(Entity& entity, EventRelay& event_relay);
	};

	class refl_ TOY_CORE_EXPORT EventReceptor : public Receptor, public EventTubeEnd, public StoreObserver<Entity>
	{
	public:
		EventReceptor(Entity& entity, EventRelay& event_relay);

		void add_scope(Medium& medium, float radius, CollisionGroup group = CM_RECEPTOR);
		void removeScope(ReceptorScope& scope);

		void handle_add(Entity& emitter);
		void handle_remove(Entity& emitter);
	};
}
