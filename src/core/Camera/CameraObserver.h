//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>
#include <math/Vec.h>

using namespace mud; namespace toy
{
	class CameraObserver { public: virtual void cameraUpdated(const vec3& lensPosition, const quat& lensRotation, const vec3& target, float nearClip, float farClip) = 0; };
}
