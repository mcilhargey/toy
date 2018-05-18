#pragma once

#include <space/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>
#include <util/Generated/Types.h>
#include <core/Generated/Types.h>
#include <db/Generated/Types.h>
#include <ui/Generated/Types.h>
#include <uio/Generated/Types.h>
#include <gfx/Generated/Types.h>
#include <visu/Generated/Types.h>
#include <edit/Generated/Types.h>
#include <block/Generated/Types.h>
#include <gen/Generated/Types.h>
#include <shell/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    template <> _EX_SPACE_EXPORT Type& type<FleetStance>();
    template <> _EX_SPACE_EXPORT Type& type<WeaponType>();
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Commander*>>();
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Fleet*>>();
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Quadrant*>>();
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Star*>>();
    
    template <> _EX_SPACE_EXPORT Type& type<Commander>();
    template <> _EX_SPACE_EXPORT Type& type<FleetJump>();
    template <> _EX_SPACE_EXPORT Type& type<ShipComponent>();
    template <> _EX_SPACE_EXPORT Type& type<ShipEngine>();
    template <> _EX_SPACE_EXPORT Type& type<ShipHull>();
    template <> _EX_SPACE_EXPORT Type& type<ShipSchema>();
    template <> _EX_SPACE_EXPORT Type& type<SpatialCombat>();
    template <> _EX_SPACE_EXPORT Type& type<SpatialPower>();
    template <> _EX_SPACE_EXPORT Type& type<CommanderBrush>();
    template <> _EX_SPACE_EXPORT Prototype& proto<Star>();
    template <> _EX_SPACE_EXPORT Type& type<Star>();
    template <> _EX_SPACE_EXPORT Prototype& proto<Fleet>();
    template <> _EX_SPACE_EXPORT Type& type<Fleet>();
    template <> _EX_SPACE_EXPORT Prototype& proto<Quadrant>();
    template <> _EX_SPACE_EXPORT Type& type<Quadrant>();
    template <> _EX_SPACE_EXPORT Prototype& proto<Galaxy>();
    template <> _EX_SPACE_EXPORT Type& type<Galaxy>();
    template <> _EX_SPACE_EXPORT Prototype& proto<Universe>();
    template <> _EX_SPACE_EXPORT Type& type<Universe>();
}
