#pragma once

#include <editor/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>
#include <geom/Generated/Types.h>
#include <lang/Generated/Types.h>
#include <util/Generated/Types.h>
#include <core/Generated/Types.h>
#include <ui/Generated/Types.h>
#include <uio/Generated/Types.h>
#include <gfx/Generated/Types.h>
#include <visu/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    
    template <> TOY_EDITOR_EXPORT Type& type<toy::ActionGroup>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Clone>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Cut>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Edit>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Editor>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Paste>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Toolbelt>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::Toolbox>();
    template <> TOY_EDITOR_EXPORT Type& type<toy::DynamicToolbox>();
}
