

#include <space/Generated/Types.h>
#include <space/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    template <> _EX_SPACE_EXPORT Type& type<FleetStance>() { static Type ty("FleetStance"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<FleetJump::State>() { static Type ty("FleetJump::State"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<SpatialCombat::State>() { static Type ty("SpatialCombat::State"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<WeaponType>() { static Type ty("WeaponType"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Commander*>>() { static Type ty("std::vector<Commander*>"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Fleet*>>() { static Type ty("std::vector<Fleet*>"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Quadrant*>>() { static Type ty("std::vector<Quadrant*>"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<std::vector<Star*>>() { static Type ty("std::vector<Star*>"); return ty; }
    
    template <> _EX_SPACE_EXPORT Type& type<Commander>() { static Type ty("Commander"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<FleetJump>() { static Type ty("FleetJump"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<ShipComponent>() { static Type ty("ShipComponent"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<ShipEngine>() { static Type ty("ShipEngine"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<ShipHull>() { static Type ty("ShipHull"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<ShipSchema>() { static Type ty("ShipSchema"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<SpatialCombat>() { static Type ty("SpatialCombat"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<SpatialPower>() { static Type ty("SpatialPower"); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<CommanderBrush>() { static Type ty("CommanderBrush", type<mud::Brush>()); return ty; }
    template <> _EX_SPACE_EXPORT Prototype& proto<Star>() { static Prototype ty("Star", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::Receptor>(), &type<toy::Active>() }); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<Star>() { return proto<Star>(); }
    template <> _EX_SPACE_EXPORT Prototype& proto<Fleet>() { static Prototype ty("Fleet", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::Receptor>(), &type<toy::Active>() }); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<Fleet>() { return proto<Fleet>(); }
    template <> _EX_SPACE_EXPORT Prototype& proto<Quadrant>() { static Prototype ty("Quadrant", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::WorldPage>(), &type<toy::BufferPage>() }); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<Quadrant>() { return proto<Quadrant>(); }
    template <> _EX_SPACE_EXPORT Prototype& proto<Galaxy>() { static Prototype ty("Galaxy", type<toy::Entity>(), {  }); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<Galaxy>() { return proto<Galaxy>(); }
    template <> _EX_SPACE_EXPORT Prototype& proto<Universe>() { static Prototype ty("Universe", type<toy::World>(), { &type<toy::BulletWorld>() }); return ty; }
    template <> _EX_SPACE_EXPORT Type& type<Universe>() { return proto<Universe>(); }
}
