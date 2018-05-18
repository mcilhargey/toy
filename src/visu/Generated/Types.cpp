

#include <visu/Generated/Types.h>
#include <visu/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> TOY_VISU_EXPORT Type& type<toy::SoundSource>() { static Type ty("SoundSource"); return ty; }
    template <> TOY_VISU_EXPORT Type& type<toy::VisuScene>() { static Type ty("VisuScene"); return ty; }
    template <> TOY_VISU_EXPORT Type& type<toy::VisuSystem>() { static Type ty("VisuSystem"); return ty; }
    template <> TOY_VISU_EXPORT Type& type<toy::VisuModule>() { static Type ty("VisuModule", type<mud::Module>()); return ty; }
}
