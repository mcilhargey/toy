//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <core/Generated/Forward.h>

using namespace mud; namespace toy
{
	class MotionStateObserver { public:	virtual void update(MotionState* motionState) = 0; };
}
