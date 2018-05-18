//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <visu/VisuScene.h>

#include <visu/VisuSystem.h>
#include <visu/VisuModule.h>

#include <obj/Reflect/Member.h>
#include <geom/Shapes.h>
#include <geom/ShapesComplex.h>

#include <core/World/Section.h>
#include <core/World/World.h>
#include <core/Entity/Entity.h>
#include <core/Physic/Obstacle.h>
#include <core/Disq/Disq.h>
#include <core/WorldPage/WorldPage.h>
#include <core/Symbolic/Symbolic.h>
#include <core/Light/Light.h>
#include <core/Active/Active.h>
#include <core/Active/Effect.h>

#include <core/View/View.h>
#include <core/View/Vision.h>
#include <core/Selector/Selection.h>

//#include <core/Buffer/BufferView.h>

#include <core/Navmesh/Navmesh.h>
//#include <visu/Ogre/FacetNavmesh/OgreNavmesh.h>

#include <gfx/Gfx.h>
#include <gfx/Item.h>
#include <gfx/GfxSystem.h>
#include <gfx/Draw.h>
#include <gfx/Camera.h>
#include <gfx/Frustum.h>

#include <snd/SoundManager.h>

#include <obj/Util/Timer.h>

#define TOY_PRIVATE
#include <core/Bullet.h>

#include <gfx/Material.h>
#include <gfx/Item.h>
#include <bgfx/bgfx.h>
#include <core/Bullet/BulletWorld.h>
#include <core/Physic/Solid.h>

#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>

#ifdef TOY_SOUND
#include <snd/SoundManager.h>
#include <snd/Sound.h>
#endif

using namespace mud; namespace toy
{
#ifdef TOY_PHYSIC_DEBUG_DRAW
	class VisuScene::PhysicDebugDraw : public btIDebugDraw
	{
	public:
		PhysicDebugDraw(ImmediateDraw& immediate) : m_immediate(immediate) {}

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) final
		{
			m_immediate.draw(bxidentity(), { { Symbol(to_colour(color)), Line(to_vec3(from), to_vec3(to)), OUTLINE, Zero3  } });
		}

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
		{
			UNUSED(PointOnB); UNUSED(normalOnB); UNUSED(distance); UNUSED(lifeTime); UNUSED(color);
		}

		virtual void reportErrorWarning(const char* warningString)
		{
			UNUSED(warningString);
		}

		virtual void draw3dText(const btVector3& location, const char* textString)
		{
			UNUSED(location); UNUSED(textString);
		}

		virtual void setDebugMode(int debugMode)
		{
			UNUSED(debugMode);
		}

		virtual int	getDebugMode() const
		{
			// DBG_NoDebug = 0,
			// DBG_DrawWireframe = 1,
			// DBG_DrawAabb = 2,
			// DBG_DrawFeaturesText = 4,
			// DBG_DrawContactPoints = 8,
			// DBG_NoDeactivation = 16,
			// DBG_NoHelpText = 32,
			// DBG_DrawText = 64,
			// DBG_ProfileTimings = 128,
			// DBG_EnableSatComparison = 256,
			// DBG_DisableBulletLCP = 512,
			// DBG_EnableCCD = 1024,
			// DBG_DrawConstraints = (1 << 11),
			// DBG_DrawConstraintLimits = (1 << 12),
			// DBG_FastWireframe = (1 << 13),
			// DBG_DrawNormals = (1 << 14),
			// DBG_DrawFrames = (1 << 15),
			return DBG_DrawWireframe;
			//return DBG_DrawAabb;
		}

		ImmediateDraw& m_immediate;
	};
#endif

	VisuScene::VisuScene(Vision& vision, VisuSystem& visu_system)
		: m_vision(vision)
		, m_world(vision.m_world)
		, m_visu_system(visu_system)
		, m_gfx_system(*visu_system.m_gfx_system)
		, m_scene(*visu_system.m_gfx_system)
#ifdef TOY_SOUND
		, m_snd_manager(*visu_system.m_sound_system)
#endif
#ifdef TOY_PHYSIC_DEBUG_DRAW
		, m_debug_draw(make_unique<PhysicDebugDraw>(*m_scene.m_immediate))
#endif
    {
		m_world.addTask(this, short(Task::Graphics));

		for(VisuModule* module : visu_system.m_modules)
			module->startScene(*this);

#ifdef TOY_PHYSIC_DEBUG_DRAW
		SubBulletWorld& bullet_world = as<SubBulletWorld>(vision.m_world.part<BulletWorld>().getSubWorld(SolidMedium::me()));
		bullet_world.m_bulletWorld->setDebugDrawer(m_debug_draw.get());
#endif
	}

	VisuScene::~VisuScene()
    {
		m_world.removeTask(this, short(Task::Graphics));
    }

	void VisuScene::paint_entity(Gnode& parent, Entity& entity)
	{
		Gnode& node = gfx::node(parent, &entity, entity.absolutePosition(), entity.absoluteRotation());
		for(auto& painter : m_entity_painters)
			if(painter->m_filterEntity(entity))
				painter->m_paintEntity(node.sub((void*) painter->m_index), entity);
	}

	void VisuScene::next_frame(size_t tick, size_t delta)
	{
#ifdef TOY_PHYSIC_DEBUG_DRAW
		SubBulletWorld& bullet_world = as<SubBulletWorld>(m_vision.m_world.part<BulletWorld>().getSubWorld(SolidMedium::me()));
		bullet_world.m_bulletWorld->debugDrawWorld();
#endif

		UNUSED(tick); UNUSED(delta);

#ifdef TOY_SOUND
		m_snd_manager.threadUpdate();
#endif

		for(VisuCamera& visucamera : m_cameras)
			update_camera(*visucamera.m_camera, *visucamera.m_gfx_camera);

		Gnode& root = m_scene.begin();

		for(size_t i = 0; i < m_painters.size(); ++i)
			m_painters[i]->m_paint(i, *this, root);

		Gnode& entity_node = root.sub((void*) (m_painters.size() + 1));
		for(auto& entity_num : m_vision.m_entities.store())
			if(entity_num.first)
				paint_entity(entity_node.sub((void*) entity_num.first->m_id), *entity_num.first);
	}

	EntityPainter::EntityPainter(cstring name, size_t index, std::function<bool(Entity&)> filterEntity, std::function<void(Gnode&, Entity&)> paintEntity)
		: VisuPainter(name, index, {})
		, m_filterEntity(filterEntity)
		, m_paintEntity(paintEntity)
	{}

	void paint_selected(Gnode& parent, Entity& entity)
	{
		UNUSED(entity);
		//if(!parent.m_focused)
			return;

		Aabb aabb;
#if 0
		for(Item* item : parent.m_attach->m_items)
		{
			if(item->m_instances.empty())
				aabb.mergeSafe(item->m_model->m_aabb);
			//else
			//	for(mat4& mat : item->m_instances)
			//		aabb.mergeSafe(mat * aabb)
		}
#endif
		gfx::shape(parent, aabb, Symbol(Colour::White));
	}

	void visu_selection(Gnode& parent, Selection& selection)
	{
		UNUSED(selection);
		return;
		Aabb bounds;
		/*for(Entity* entity : selection.m_entities.store())
			for(Item* item : parent.m_items)
			{
				Aabb aabb = item->m_model.m_aabb;
				aabb.m_center += parent.m_position;
				bounds.mergeSafe(aabb);
			}*/

		gfx::shape(parent, bounds, Symbol(Colour::White));
	}

	void update_camera(Camera& camera, Camera& gfx_camera)
	{
		gfx_camera.set_look_at(camera.m_lensPosition, camera.m_entity.absolutePosition());

		gfx_camera.m_near = camera.m_nearClipDistance;
		gfx_camera.m_far = camera.m_farClipDistance;
	}

	void paint_frustum(Gnode& parent, Frustum& frustum)
	{
		gfx::shape(parent, Box({ &frustum.m_corners[0], 8 }), Symbol());
	}

	void paint_camera(Gnode& parent, Camera& camera)
	{
		UNUSED(camera);
		gfx::shape(parent, Circle(0.01f), Symbol(Colour::White));
		//gfx::shape(parent, Spheroid(1.f), Symbol(Colour::White));
	}

	void paint_light(Gnode& parent, LightSource& light)
	{
		gfx::light(parent, LightType::Point, light.m_shadows, light.m_colour, light.m_range);

#ifdef DEBUG_LIGHT
		gfx::shape(parent, Spheroid(0.2f), Symbol(Colour::White));
		gfx::shape(parent, Circle(light.m_range), Symbol(Colour::White));
#endif
	}

	void paint_symbolic(Gnode& parent, Symbolic& symbolic)
	{
		UNUSED(parent); UNUSED(symbolic);
		//for(auto& symbol : symbolic.m_symbols)
		//	gfx::shape(parent, symbol.m_shape, symbol);
	}

	void paint_obstacle(Gnode& parent, Obstacle& obstacle)
	{
		UNUSED(obstacle);
		//ConvexHullShape& colShape = as<ConvexHullShape>(obstacle.shape());
		gfx::shape(parent, Box(/*(colShape.points())*/), Symbol(Colour::Pink));
	}

	void paint_disq(Gnode& parent, Disq& disq)
	{
		gfx::shape(parent, Circle(disq.m_radius, Axis::X), Symbol(Colour::Pink));
	}

	void paint_event_sphere(Gnode& parent, EventReceptor& receptor)
	{
		UNUSED(receptor);
		float size = 0.f; // receptor.sphere.radius
		gfx::shape(parent, Spheroid(size), Symbol(Colour(1.f, 0.f, 1.f, 0.2f)));
	}

	void paint_entity(Gnode& parent, Entity& entity)
	{
		UNUSED(parent); UNUSED(entity);
		// if obstructed
	}

	void advance_animation(const string& name, bool loop, float speed)
	{
		UNUSED(loop); UNUSED(speed);
		string capname = name;
		capname[0] = toupper(capname[0]);

		// todo
	}

	void paint_active(Gnode& parent, Active& active)
	{
		UNUSED(parent);

		for(Effect& effect : active.m_effects)
			advance_animation(effect.m_name, effect.m_loop, 1.f);

		for(State& state : active.m_states)
			advance_animation(state.m_name, true, 1.f);
	}

#ifdef TOY_SOUND
	void snd_sound(Gnode& parent, const string& sound, bool loop)
	{
		if(parent.m_sound) return;
		parent.m_sound = parent.m_sound_manager->createSound((sound + ".ogg").c_str(), loop, false, [](Sound&) {});
		if(parent.m_sound)
		{
			parent.m_sound->play();
			parent.m_sound->enable3D();
		}
		//sound->stop();
	}
#else
	void snd_sound(Gnode& parent, const string& sound, bool loop)
	{
		UNUSED(parent); UNUSED(sound); UNUSED(loop);
	}
#endif

	// @todo
	// sub entities bone attachments
	// selected materials or bounding box
	// obstructing materials : set a variant of material with alpha blending when between camera and actor
	// depending on a SHADEABLE_OGRE_MASK
}
