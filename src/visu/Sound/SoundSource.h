//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Store/StoreObserver.h>
#include <core/Entity/EntityObserver.h>
#include <visu/Generated/Forward.h>

/* std */
#include <map>

using namespace mud; namespace toy
{
	class _refl_ TOY_VISU_EXPORT SoundSource : public StoreObserver<State>
	{
	public:
#ifdef TOY_SOUND
		SoundSource(Entity& entity, SoundManager& soundManager);
#else
		SoundSource(Entity& entity);
#endif

		_attr_ Entity& m_entity;

#ifdef TOY_SOUND
		void soundDestroyed(Sound& sound);
#endif

		void handleAdd(State& effect);
		void handleRemove(State& effect);

	private:
		Active& m_active;

#ifdef TOY_SOUND
		SoundManager& m_soundManager;
		std::map<State*, Sound*> m_sounds;
#endif

		size_t m_updated;
	};
}
