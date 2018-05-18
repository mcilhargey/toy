

#include <platform/Generated/Types.h>
#include <platform/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> _EX_PLATFORM_EXPORT Prototype& proto<Worldblock>() { static Prototype ty("Worldblock", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::WorldPage>(), &type<toy::Navblock>() }); return ty; }
    template <> _EX_PLATFORM_EXPORT Type& type<Worldblock>() { return proto<Worldblock>(); }
    template <> _EX_PLATFORM_EXPORT Prototype& proto<TileWorld>() { static Prototype ty("TileWorld", type<toy::World>(), { &type<toy::BulletWorld>(), &type<toy::Navmesh>() }); return ty; }
    template <> _EX_PLATFORM_EXPORT Type& type<TileWorld>() { return proto<TileWorld>(); }
    template <> _EX_PLATFORM_EXPORT Prototype& proto<Human2>() { static Prototype ty("Human2", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::Receptor>(), &type<toy::Active>() }); return ty; }
    template <> _EX_PLATFORM_EXPORT Type& type<Human2>() { return proto<Human2>(); }
}
