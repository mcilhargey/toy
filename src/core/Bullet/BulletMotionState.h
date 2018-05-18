//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifdef TOY_PRIVATE

/* toy */
#include <math/Vec.h>
#include <core/Generated/Forward.h>
#include <core/Movable/MotionState.h>

/* bullet */
class TOY_CORE_EXPORT btMotionState;
#include <LinearMath/btMotionState.h>

using namespace mud; namespace toy
{
    class TOY_CORE_EXPORT BulletMotionState : public btMotionState
    {
    public:
        BulletMotionState(MotionState& motionState, const vec3& offset = Zero3);

		virtual void getWorldTransform(btTransform& worldTransform) const;
		virtual void setWorldTransform(const btTransform& worldTransform);

	protected:
		MotionState& m_motionState;
		vec3 m_offset;
    };
}

#endif
