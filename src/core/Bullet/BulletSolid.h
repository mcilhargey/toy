//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Generated/Forward.h>
#include <core/Bullet/BulletCollider.h>

using namespace mud; namespace toy
{
	class BulletMotionState;

	class _refl_ TOY_CORE_EXPORT BulletSolid : public BulletCollider
    {
    public:
		BulletSolid(SubBulletWorld& bulletWorld, Solid& solid);
        ~BulletSolid();

		btRigidBody* m_rigidBody;

		virtual void updateTransform(const vec3& position, const quat& rotation) override;

		virtual void setForce(const vec3& force) override;
		virtual void setTorque(const vec3& torque) override;

    public:
		unique_ptr<BulletMotionState> m_motionState;
    };
}
