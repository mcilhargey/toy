

#include <05_character/Generated/Types.h>
#include <05_character/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> _05_CHARACTER_EXPORT Type& type<Walk>() { static Type ty("Walk"); return ty; }
    template <> _05_CHARACTER_EXPORT Prototype& proto<Human>() { static Prototype ty("Human", type<toy::Entity>(), { &type<toy::Movable>(), &type<toy::Manoeuvrable>(), &type<toy::Emitter>(), &type<toy::Receptor>(), &type<toy::Active>(), &type<toy::Actor>(), &type<toy::Reactive>() }); return ty; }
    template <> _05_CHARACTER_EXPORT Type& type<Human>() { return proto<Human>(); }
}
