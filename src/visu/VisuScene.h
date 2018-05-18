//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Graph/Node.h>
#include <obj/Util/Timer.h>
#include <obj/Util/Updatable.h>
#include <visu/Generated/Forward.h>

#include <core/Entity/Entity.h>
#include <core/Camera/Camera.h>

#include <gfx/Scene.h>

#include <functional>
#include <string>

#define TOY_PHYSIC_DEBUG_DRAW

using namespace mud; namespace toy
{
	using string = std::string;

	struct TOY_VISU_EXPORT VisuPainter
	{
		VisuPainter(cstring name, size_t index, std::function<void(size_t, VisuScene&, Gnode&)> paint)
			: m_name(name)
			, m_index(index)
			, m_enabled(true)
			, m_filterView([](View&) { return true; })
			, m_paint(paint)
		{}

		string m_name;
		size_t m_index;
		bool m_enabled;

		std::function<bool(View&)> m_filterView;
		std::function<void(size_t, VisuScene&, Gnode&)> m_paint;
	};

	struct TOY_VISU_EXPORT EntityPainter : public VisuPainter
	{
		EntityPainter(cstring name, size_t index, 
					  std::function<bool(Entity&)> filterEntity, std::function<void(Gnode&, Entity&)> paintEntity);

		std::function<bool(Entity&)> m_filterEntity;
		std::function<void(Gnode&, Entity&)> m_paintEntity;
	};

	struct TOY_VISU_EXPORT VisuCamera
	{
		Camera* m_camera;
		Camera* m_gfx_camera;
	};

	struct TOY_VISU_EXPORT SceneState
	{

	};

	class _refl_ TOY_VISU_EXPORT VisuScene : public NonCopy, public Updatable
    {
    public:
        VisuScene(Vision& vision, VisuSystem& visu_system);
        ~VisuScene();

		_attr_ Vision& m_vision;
		_attr_ World& m_world;
		_attr_ VisuSystem& m_visu_system;
		_attr_ GfxSystem& m_gfx_system;
		_attr_ Scene m_scene;

#ifdef TOY_SOUND
		SoundManager& m_snd_manager;
#endif

#ifdef TOY_PHYSIC_DEBUG_DRAW
		class PhysicDebugDraw;
		unique_ptr<PhysicDebugDraw> m_debug_draw;
#endif

		std::vector<unique_ptr<VisuPainter>> m_painters;
		std::vector<unique_ptr<EntityPainter>> m_entity_painters;

		std::vector<VisuCamera> m_cameras;

		void next_frame(size_t tick, size_t delta);

		void paint_entity(Gnode& parent, Entity& entity);

		inline void painter(cstring name, std::function<void(size_t, VisuScene&, Gnode&)> paint)
		{
			m_painters.emplace_back(make_unique<VisuPainter>(name, m_painters.size(), paint));
		}

		SceneState* m_scene_state = nullptr;

	private:
		Clock m_clock;
    };

	template <class T_Part, void T_PaintFunc(Gnode&, T_Part&)>
	unique_ptr<EntityPainter> partPainter(const string& name, size_t index)
	{
		auto filterEntity = [](Entity& entity) -> bool { return entity.has_part(type<T_Part>()); };
		auto paintEntity = [](Gnode& parent, Entity& entity) { T_PaintFunc(parent, entity.part<T_Part>()); };
		return make_unique<EntityPainter>(name, index, filterEntity, paintEntity);
	};
;
	TOY_VISU_EXPORT void update_camera(Camera& camera, Camera& gfx_camera);

	TOY_VISU_EXPORT void paint_selected(Gnode& parent, Entity& entity);
	TOY_VISU_EXPORT void paint_camera(Gnode& parent, Camera& camera);
	TOY_VISU_EXPORT void paint_light(Gnode& parent, LightSource& light);
	TOY_VISU_EXPORT void paint_symbolic(Gnode& parent, Symbolic& symbolic);
	TOY_VISU_EXPORT void paint_obstacle(Gnode& parent, Obstacle& obstacle);
	TOY_VISU_EXPORT void paint_disq(Gnode& parent, Disq& disq);
	TOY_VISU_EXPORT void paint_event_sphere(Gnode& parent, EventReceptor& receptor);
	TOY_VISU_EXPORT void paint_entity(Gnode& parent, Entity& entity);
	TOY_VISU_EXPORT void paint_active(Gnode& parent, Active& active);

	TOY_VISU_EXPORT void snd_sound(Gnode& parent, const string& sound, bool loop = false);
}
