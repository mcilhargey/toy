#pragma once

#include <util/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    
    template <> TOY_UTIL_EXPORT Type& type<toy::Loader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::Procedure>();
    template <> TOY_UTIL_EXPORT Type& type<toy::ProcedureType>();
    template <> TOY_UTIL_EXPORT Type& type<toy::ObjectLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::MemberLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::TypeLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::LinkedLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::SequenceLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::StructureLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::PartLoader>();
    template <> TOY_UTIL_EXPORT Type& type<toy::OwnedLoader>();
}
