

#include <blocks/Generated/Types.h>
#include <blocks/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    template <> _EX_BLOCKS_EXPORT Type& type<std::vector<toy::Element*>>() { static Type ty("std::vector<toy::Element*>"); return ty; }
    
    template <> _EX_BLOCKS_EXPORT Prototype& proto<BlockWorld>() { static Prototype ty("BlockWorld", type<toy::World>(), { &type<toy::BulletWorld>(), &type<toy::Navmesh>() }); return ty; }
    template <> _EX_BLOCKS_EXPORT Type& type<BlockWorld>() { return proto<BlockWorld>(); }
    template <> _EX_BLOCKS_EXPORT Prototype& proto<Crate>() { static Prototype ty("Crate", type<toy::Entity>(), { &type<toy::Movable>(), &type<toy::Emitter>(), &type<toy::Active>() }); return ty; }
    template <> _EX_BLOCKS_EXPORT Type& type<Crate>() { return proto<Crate>(); }
}
