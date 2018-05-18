//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Store/Array.h>
#include <core/Store/CountStore.h>
#include <core/Entity/EntityObserver.h>
#include <core/View/View.h>
#include <core/Generated/Types.h> // @kludge because of Array

using namespace mud; namespace toy
{
	class _refl_ TOY_CORE_EXPORT Vision : public StoreObserver<Entity>
	{
	public:
		_constr_ Vision(World& world);
		~Vision();

		_attr_ World& m_world;

		Chained<CountStore<Entity>> m_entities;
		Array<Camera> m_cameras;
		Array<View> m_views;

		void handleAdd(Entity& owned);
		void handleRemove(Entity& owned);

		void addView(View& view);

		Camera& addCamera(const vec3& position, float lensDistance, bool planar);
	};

	class _refl_ TOY_CORE_EXPORT PlayerVision : public Vision
	{
	public:
		PlayerVision(Player& player, World& world);

		//StoreView m_ownedView;
		//ReceptorView m_visualView;
		//ReceptorView m_soundView;
		//ReceptorView m_worldView;
		//ReceptorView m_bufferView;
	};

	class _refl_ TOY_CORE_EXPORT OmniVision : public Vision
	{
	public:
		OmniVision(World& world);
		~OmniVision();

		Array<Entity> m_origin;
		RecursiveStore m_store;
		StoreView m_view;
	};
}
