

#pragma once

#include <obj/Config.h>

#include <obj/Generated/Forward.h>
#include <math/Generated/Forward.h>
#include <util/Generated/Forward.h>
#include <core/Generated/Forward.h>
#include <db/Generated/Forward.h>
#include <ui/Generated/Forward.h>
#include <uio/Generated/Forward.h>
#include <gfx/Generated/Forward.h>
#include <visu/Generated/Forward.h>
#include <edit/Generated/Forward.h>
#include <block/Generated/Forward.h>
#include <gen/Generated/Forward.h>
#include <shell/Generated/Forward.h>

#if defined _EX_SPACE_LIB
#define _EX_SPACE_EXPORT MUD_EXPORT
#else
#define _EX_SPACE_EXPORT MUD_IMPORT
#endif


    enum class FleetStance : unsigned int;
    enum class WeaponType : unsigned int;
    
    
    struct SpatialPower;
    class Star;
    struct FleetJump;
    class Fleet;
    class ShipHull;
    class ShipEngine;
    class ShipComponent;
    struct ShipSchema;
    class ShipDatabase;
    class Commander;
    class Quadrant;
    struct GalaxyGrid;
    struct SpatialCombat;
    class Galaxy;
    class Universe;
    class CommanderBrush;

namespace std {

    
    
}

namespace mud {
namespace ui {

    
    
}
}

namespace glm {

    
    
}

namespace bgfx {

    
    
}

namespace mud {
namespace gfx {

    
    
}
}

namespace mud {

    
    
    class Dice100;
    class Dice10;
}

namespace mud {
namespace detail {

    
    
}
}

namespace bimg {

    
    
}

namespace toy {

    
    
}

namespace bx {

    
    
}

