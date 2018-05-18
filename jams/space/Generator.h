
#pragma once

#include <math/Vec.h>
#include <space/Generated/Forward.h>

#include <vector>

using namespace mud;
using namespace toy;

_EX_SPACE_EXPORT _func_ Star* generate_system(Entity& origin, const uvec3& coord, const vec3& position);
_EX_SPACE_EXPORT _func_ Fleet* generate_fleet(Entity& origin, const uvec3& coord, const vec3& position);
_EX_SPACE_EXPORT _func_ Commander* generate_commander(Entity& origin, Star& star);
_EX_SPACE_EXPORT _func_ Star* assign_system(Entity& origin, Star& star, std::vector<Commander*> commanders);

void space_generator(GameShell& shell, VisualScript& script);
VisualScript& space_generator(GameShell& shell);
