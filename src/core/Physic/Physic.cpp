//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <core/Entity/Entity.h>
#include <core/Entity/Entity.h>
#include <core/Physic/Physic.h>
#include <core/Physic/CollisionShape.h>

using namespace mud; namespace toy
{
	PhysicMedium::PhysicMedium()
		: Medium("PhysicMedium", false)
	{
		m_masks[CM_OBJECT] = CM_BUFFER | CM_AREA;
		m_masks[CM_BUFFER] = CM_OBJECT;
		m_masks[CM_AREA] = CM_OBJECT;
	}

	Physic::Physic(Entity& entity)
		//: m_areaStore()
	{
		UNUSED(entity);
		//entity.part<Emitter>().addEmitter(PhysicMedium::me(), make_object<SphereShape>(0.1f), CM_OBJECT);
	}

    /*void Physic::handleAdd(Collider& object)
    {
		m_areaStore.add(object->m_entity.part<Area>());
	}

    void Physic::handleRemove(Collider& object)
    {
		m_areaStore.add(object->m_entity.part<Area>());
    }*/
}
