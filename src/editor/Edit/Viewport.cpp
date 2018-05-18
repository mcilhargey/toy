//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Edit/Viewport.h>

#include <edit/Viewport.h>

#include <gfx/Widget/Viewer.h>
#include <gfx/Item.h>

#include <core/Camera/Camera.h>
#include <core/View/Vision.h>
#include <core/Selector/Selector.h>
#include <core/World/World.h>
#include <core/Physic/PhysicWorld.h>

#include <uio/Edit/Section.h>
#include <visu/VisuScene.h>

#include <editor/Controller/RTSCameraController.h>

using namespace mud; namespace toy
{
	vec3 pick_terrain(Viewer& viewer, World& world, vec2 position)
	{
		Ray ray = viewer.m_camera.ray(position);
		return world.part<PhysicWorld>().groundPoint(ray);
	}

	Widget& scene_viewport(Widget& parent, VisuScene& scene, Camera& camera, Selection& selection)
	{
		Viewer& viewer = parent.child_args<Viewer, Scene&>(scene.m_scene);
		if(viewer.once())
		{
			scene.m_cameras.push_back({ &camera, &viewer.m_camera });
			viewer.m_controller = make_unique<RTSCameraController>(viewer, camera);
		}

		viewer.m_controller->process(viewer);
		viewport_picker(viewer, viewer, selection);
		return viewer;
	}

	void scene_vision(Widget& parent, VisuScene& scene, Selection& selection)
	{
		Camera& camera = scene.m_vision.addCamera(vec3(50.f, 0.f, 50.f), 100.f, true);
		scene_viewport(parent, scene, camera, selection);
		//m_activeViewport = &viewport;
	}
}
