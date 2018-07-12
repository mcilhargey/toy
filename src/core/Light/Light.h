//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <proto/Proto.h>
#include <math/Stat.h>
#include <math/Colour.h>
#include <core/Forward.h>
#include <core/Physic/Scope.h>
#include <core/Entity/EntityObserver.h>

using namespace mud; namespace toy
{
	//class TOY_CORE_EXPORT VisualEmitter object_ptr<LightReflector> m_lreflector;

	class refl_ TOY_CORE_EXPORT LightReflector
	{
	public:
		LightReflector(Entity& entity, Receptor& receptor);

		attr_ Entity& m_entity;
		attr_ Ratio m_brightness;

		virtual void handleMoved();

	protected:
		ReceptorSphere& m_sphere;
	};

	class refl_ TOY_CORE_EXPORT LightSource : public StoreObserver<Entity>
	{
	public:
		constr_ LightSource(Entity& entity, Emitter& emitter, float range, float intensity, Colour colour, bool shadows);

		attr_ Entity& m_entity;
		attr_ Emitter& m_emitter;
		attr_ mut_ float m_range;
		attr_ mut_ float m_intensity;
		attr_ mut_ Colour m_colour;
		attr_ mut_ bool m_shadows;

		virtual void handle_add(Entity& contact);
		virtual void handle_remove(Entity& contact);

		void visualTransformUpdated(LightReflector& reflector);

		virtual void handleMoved();

	private:
		float computeIntensity(float distance);

	private:
		EmitterSphere& m_sphere;
	};

	class refl_ TOY_CORE_EXPORT OLight : public Complex
	{
	public:
		constr_ OLight(Id id, Entity& parent, const vec3& position, float range = 10.f, float intensity = 1.f, Colour colour = Colour::White, bool shadows = false);

		comp_ attr_ Entity m_entity;
		comp_ attr_ Emitter m_emitter;
		comp_ attr_ LightSource m_light;
	};
}
