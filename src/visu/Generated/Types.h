#pragma once

#include <visu/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>
#include <geom/Generated/Types.h>
#include <util/Generated/Types.h>
#include <core/Generated/Types.h>
#include <ui/Generated/Types.h>
#include <gfx/Generated/Types.h>
#include <snd/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    
    template <> TOY_VISU_EXPORT Type& type<toy::SoundSource>();
    template <> TOY_VISU_EXPORT Type& type<toy::VisuScene>();
    template <> TOY_VISU_EXPORT Type& type<toy::VisuSystem>();
    template <> TOY_VISU_EXPORT Type& type<toy::VisuModule>();
}
