//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <obj/Proto.h>
#include <math/Stat.h>
#include <math/Colour.h>
#include <core/Generated/Forward.h>
#include <core/Physic/Scope.h>
#include <core/Entity/EntityObserver.h>

using namespace mud; namespace toy
{
	//class TOY_CORE_EXPORT VisualEmitter object_ptr<LightReflector> m_lreflector;

	class _refl_ TOY_CORE_EXPORT LightReflector
	{
	public:
		LightReflector(Entity& entity);

		_attr_ Entity& m_entity;
		_attr_ Ratio m_brightness;

		virtual void handleMoved();

	protected:
		ReceptorSphere& m_sphere;
	};

	class _refl_ TOY_CORE_EXPORT LightSource : public StoreObserver<Entity>
	{
	public:
		_constr_ LightSource(Entity& entity, Emitter& emitter, float range, float intensity, Colour colour, bool shadows);

		_attr_ Entity& m_entity;
		_attr_ Emitter& m_emitter;
		_attr_ _mut_ float m_range;
		_attr_ _mut_ float m_intensity;
		_attr_ _mut_ Colour m_colour;
		_attr_ _mut_ bool m_shadows;

		virtual void handleAdd(Entity& contact);
		virtual void handleRemove(Entity& contact);

		void visualTransformUpdated(LightReflector& reflector);

		virtual void handleMoved();

	private:
		float computeIntensity(float distance);

	private:
		EmitterSphere& m_sphere;
	};

	class _refl_ TOY_CORE_EXPORT OLight : public Construct
	{
	public:
		_constr_ OLight(Id id, Entity& parent, const vec3& position, float range = 10.f, float intensity = 1.f, Colour colour = Colour::White, bool shadows = false);

		_comp_ _attr_ Entity m_entity;
		_comp_ Emitter m_emitter;
		_comp_ LightSource m_light;
	};
}
