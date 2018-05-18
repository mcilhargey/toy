#pragma once

#include <block/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>
#include <geom/Generated/Types.h>
#include <util/Generated/Types.h>
#include <core/Generated/Types.h>
#include <ui/Generated/Types.h>
#include <uio/Generated/Types.h>
#include <gfx/Generated/Types.h>
#include <visu/Generated/Types.h>
#include <edit/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    template <> TOY_BLOCK_EXPORT Type& type<toy::MatterState>();
    template <> TOY_BLOCK_EXPORT Type& type<std::vector<toy::Block*>>();
    template <> TOY_BLOCK_EXPORT Type& type<std::vector<toy::Sector*>>();
    
    template <> TOY_BLOCK_EXPORT Type& type<toy::Block>();
    template <> TOY_BLOCK_EXPORT Type& type<toy::Element>();
    template <> TOY_BLOCK_EXPORT Type& type<mud::Grid<toy::Block*>>();
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::OBlock>();
    template <> TOY_BLOCK_EXPORT Type& type<toy::OBlock>();
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Chunk>();
    template <> TOY_BLOCK_EXPORT Type& type<toy::Chunk>();
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Heap>();
    template <> TOY_BLOCK_EXPORT Type& type<toy::Heap>();
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Sector>();
    template <> TOY_BLOCK_EXPORT Type& type<toy::Sector>();
}
