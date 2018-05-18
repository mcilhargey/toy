//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
/* toy */

#define TOY_PRIVATE
#include <core/Bullet/BulletMotionState.h>

#include <core/Entity/Entity.h>
#include <core/Bullet.h>

using namespace mud; namespace toy
{
    BulletMotionState::BulletMotionState(MotionState& motionState, const vec3& offset)
        : btMotionState()
		, m_motionState(motionState)
		, m_offset(offset)
    {}

	void BulletMotionState::getWorldTransform(btTransform& worldTransform) const
	{
		worldTransform.setOrigin(to_btvec3(m_motionState.m_entity.absolutePosition() + m_offset));
		worldTransform.setRotation(to_btquat(m_motionState.m_entity.absoluteRotation()));
	}

    void BulletMotionState::setWorldTransform(const btTransform& transform)
    {
		quat rotation = to_quat(transform.getRotation()) * inverse(m_motionState.m_entity.m_parent->absoluteRotation());
		vec3 position = to_vec3(transform.getOrigin()) - m_offset - m_motionState.m_entity.m_parent->absolutePosition();
		m_motionState.syncTransform(position, rotation);
    }
}
