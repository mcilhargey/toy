//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#if 0

#include <edit/Editor/Brush.h>
#include <edit/Generated/Forward.h>

#include <obj/Reflect/Method.h>
#include <obj/Reflect/Meta.h>

#include <lang/Script.h>

#include <core/Entity/Entity.h>
#include <core/World/World.h>

#include <core/Light/Light.h>

#include <edit/Editor/Editor.h>

#include <visu/VisuScene.h>

using namespace mud; namespace toy
{
	Brush::Brush(ToolContext& context, const string& name, Type& type)
		: SpatialTool(context, name, type)
	{}

	PlaceBrush::PlaceBrush(ToolContext& context)
		: Brush(context, "Place", type<PlaceBrush>())
		, m_creator(type<Entity>())
	{
		m_creator.setPrototype(type<OLight>());
	}

	ToolState PlaceBrush::start()
	{
		m_creator.injector().m_args[0] = var(size_t(0));
		m_creator.injector().m_args[1] = Ref(m_context.m_origin);
		return SpatialTool::start();
	}

	void PlaceBrush::update(const vec3& position)
	{
		Entity& entity = m_creator.create<OLight>().m_entity;
		entity.setPosition(position);
	}

	void PlaceBrush::paint(Gnode& parent)
	{
		UNUSED(parent);
	}

	CircleBrush::CircleBrush(ToolContext& context)
		: Brush(context, "Circle", type<CircleBrush>())
		, m_creator(type<Entity>())
		, m_radius(50.f)
		, m_maxSpotRadius(10.f)
		, m_accumulate(false)
	{
		m_creator.setPrototype(type<OLight>());
	}

	void CircleBrush::paint(Gnode& parent)
	{
		Gnode& self = gfx::node(self);
		gfx::shape(self, Circle(m_radius, Axis::Y), Symbol(Colour::White, Colour::AlphaGrey));
	}

	ToolState CircleBrush::start()
	{
		m_creator.injector().m_args[0] = var(size_t(0));
		m_creator.injector().m_args[1] = Ref(m_context.m_origin);
		return SpatialTool::start();
	}

	void CircleBrush::update(const vec3& position)
	{
		if(!m_accumulate)
			this->clearStroke(position);

		float side = m_radius * 2.f;
		m_distribution = make_object<Poisson>(vec2{ side }, m_maxSpotRadius);

		vec3 point;
		while(m_distribution->addPoint(m_maxSpotRadius, point))
		{
			vec3 pos(position.x + point.x - m_radius, position.y, position.z + point.y - m_radius);

			if(filter(position, pos))
			{
				OLight& light = m_creator.create<OLight>();
				light.m_entity.setPosition(pos);
				m_stroke.push_back(&light);
			}
		}
	}

	void CircleBrush::clearStroke(const vec3& center)
	{
		for(OLight*& entity : m_stroke)
			if(entity && this->filter(center, entity->m_entity.m_position))
			{
				entity->m_entity.m_parent->detach(entity->m_entity);
				m_creator.destroy(entity);
				entity = nullptr;
			}
	}

	bool CircleBrush::filter(const vec3& center, vec3& position)
	{
		return distance2(position, center) <= m_radius * m_radius;
	}

	ScriptedBrush::ScriptedBrush(ToolContext& context, VisualScript& script)
		: Brush(context, "Scripted Brush", type<ScriptedBrush>())
		, m_call(script)
		, m_script(script)
	{}

	ToolState ScriptedBrush::start()
	{
		return SpatialTool::start();
	}

	void ScriptedBrush::update(const vec3& position)
	{
		m_call.m_arguments[0] = var(position);
		m_call();
	}

	void ScriptedBrush::paint(Gnode& parent)
	{}
}
#endif