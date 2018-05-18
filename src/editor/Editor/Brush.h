//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#pragma once

/* toy */
#include <geom/Poisson.h>
#include <edit/Generated/Forward.h>
#include <edit/Tool.h>
#include <edit/Brush.h>
#include <obj/Reflect/Injector.h>

using namespace mud; namespace toy
{
#if 0
	class _refl_ TOY_EDITOR_EXPORT Brush : public SpatialTool
	{
	public:
		Brush(ToolContext& context, const string& name, Type& type);
	};

	class _refl_ TOY_EDITOR_EXPORT PlaceBrush : public Brush
	{
	public:
		PlaceBrush(ToolContext& context);

		_attr_ Creator m_creator;

		virtual ToolState start();
		virtual void update(const vec3& position);
		virtual void paint(Gnode& parent);
	};

	class OLight;

	class _refl_ TOY_EDITOR_EXPORT CircleBrush : public Brush
	{
	public:
		CircleBrush(ToolContext& context);

		_attr_ Creator m_creator;
		_attr_ _mut_ float m_radius;
		_attr_ _mut_ float m_maxSpotRadius;

		virtual ToolState start();
		virtual void update(const vec3& position);
		virtual void paint(Gnode& parent);

		void clearStroke(const vec3& center);

		bool filter(const vec3& center, vec3& position);

	protected:
		std::vector<OLight*> m_stroke;

		object_ptr<Poisson> m_distribution;

		bool m_accumulate;
	};


	class _refl_ TOY_EDITOR_EXPORT ScriptedBrush : public Brush
	{
	public:
		ScriptedBrush(ToolContext& context, VisualScript& script);

		_attr_ Call m_call;
		
		virtual ToolState start();
		virtual void update(const vec3& position);
		virtual void paint(Gnode& parent);

	protected:
		VisualScript& m_script;
	};
#endif
}
