

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module ._space;
#else
#include <space/Types.h>
#include <space/Api.h>
#include <obj/Vector.h>
//#include <proto/Proto.h>
#endif

namespace mud
{
    // Exported types
    template <> _SPACE_EXPORT Type& type<CombatType>() { static Type ty("CombatType"); return ty; }
    template <> _SPACE_EXPORT Type& type<Experience>() { static Type ty("Experience"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetSize>() { static Type ty("FleetSize"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetStance>() { static Type ty("FleetStance"); return ty; }
    template <> _SPACE_EXPORT Type& type<GameMode>() { static Type ty("GameMode"); return ty; }
    template <> _SPACE_EXPORT Type& type<Race>() { static Type ty("Race"); return ty; }
    template <> _SPACE_EXPORT Type& type<Regime>() { static Type ty("Regime"); return ty; }
    template <> _SPACE_EXPORT Type& type<Resource>() { static Type ty("Resource"); return ty; }
    template <> _SPACE_EXPORT Type& type<Stage>() { static Type ty("Stage"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetJump::State>() { static Type ty("FleetJump::State"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetSplit::State>() { static Type ty("FleetSplit::State"); return ty; }
    template <> _SPACE_EXPORT Type& type<SpatialCombat::State>() { static Type ty("SpatialCombat::State"); return ty; }
    template <> _SPACE_EXPORT Type& type<Taxation>() { static Type ty("Taxation"); return ty; }
    template <> _SPACE_EXPORT Type& type<Technology>() { static Type ty("Technology"); return ty; }
    template <> _SPACE_EXPORT Type& type<WeaponType>() { static Type ty("WeaponType"); return ty; }
    
    template <> _SPACE_EXPORT Type& type<Combat>() { static Type ty("Combat"); return ty; }
    template <> _SPACE_EXPORT Type& type<CombatFleet>() { static Type ty("CombatFleet"); return ty; }
    template <> _SPACE_EXPORT Type& type<Commander>() { static Type ty("Commander"); return ty; }
    template <> _SPACE_EXPORT Type& type<Construction>() { static Type ty("Construction"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetJump>() { static Type ty("FleetJump"); return ty; }
    template <> _SPACE_EXPORT Type& type<FleetSplit>() { static Type ty("FleetSplit"); return ty; }
    template <> _SPACE_EXPORT Type& type<PlanetaryCombat>() { static Type ty("PlanetaryCombat"); return ty; }
    template <> _SPACE_EXPORT Type& type<Player>() { static Type ty("Player"); return ty; }
    template <> _SPACE_EXPORT Type& type<Scans>() { static Type ty("Scans"); return ty; }
    template <> _SPACE_EXPORT Type& type<Schema>() { static Type ty("Schema"); return ty; }
    template <> _SPACE_EXPORT Type& type<SpatialCombat>() { static Type ty("SpatialCombat"); return ty; }
    template <> _SPACE_EXPORT Type& type<SpatialPower>() { static Type ty("SpatialPower"); return ty; }
    template <> _SPACE_EXPORT Type& type<TechDomain>() { static Type ty("TechDomain"); return ty; }
    template <> _SPACE_EXPORT Type& type<Fleet>() { static Type ty("Fleet", type<mud::Complex>()); return ty; }
    template <> _SPACE_EXPORT Type& type<Galaxy>() { static Type ty("Galaxy", type<mud::Complex>()); return ty; }
    template <> _SPACE_EXPORT Type& type<Quadrant>() { static Type ty("Quadrant", type<mud::Complex>()); return ty; }
    template <> _SPACE_EXPORT Type& type<Star>() { static Type ty("Star", type<mud::Complex>()); return ty; }
    template <> _SPACE_EXPORT Type& type<Universe>() { static Type ty("Universe", type<mud::Complex>()); return ty; }
    template <> _SPACE_EXPORT Type& type<CommanderBrush>() { static Type ty("CommanderBrush", type<mud::Brush>()); return ty; }
    template <> _SPACE_EXPORT Type& type<BuildingSchema>() { static Type ty("BuildingSchema", type<Schema>()); return ty; }
    template <> _SPACE_EXPORT Type& type<ShipComponent>() { static Type ty("ShipComponent", type<Schema>()); return ty; }
    template <> _SPACE_EXPORT Type& type<ShipEngine>() { static Type ty("ShipEngine", type<Schema>()); return ty; }
    template <> _SPACE_EXPORT Type& type<ShipHull>() { static Type ty("ShipHull", type<Schema>()); return ty; }
    template <> _SPACE_EXPORT Type& type<ShipSchema>() { static Type ty("ShipSchema", type<Schema>()); return ty; }
}
