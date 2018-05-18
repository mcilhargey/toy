#pragma once

#include <blocks/Generated/Forward.h>

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
#include <05_character/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    template <> _EX_BLOCKS_EXPORT Type& type<std::vector<toy::Element*>>();
    
    template <> _EX_BLOCKS_EXPORT Prototype& proto<BlockWorld>();
    template <> _EX_BLOCKS_EXPORT Type& type<BlockWorld>();
    template <> _EX_BLOCKS_EXPORT Prototype& proto<Crate>();
    template <> _EX_BLOCKS_EXPORT Type& type<Crate>();
}
