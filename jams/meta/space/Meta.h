

#pragma once

#ifndef MUD_MODULES
#include <meta/space/Module.h>

#include <obj/Any.h>
#include <obj/Vector.h>
#include <refl/MetaDecl.h>
#include <refl/Module.h>
#endif

namespace mud
{
    void _space_meta(Module& m)
    {   
    // Base Types
    
    // Enums
    {
        static Meta meta = { type<CombatType>(), &namspc({}), "CombatType", sizeof(CombatType), TypeClass::Enum };
        static Enum enu = { type<CombatType>(),
            true,
            { "Spatial", "Planetary" },
            { 0, 1 },
            { var(CombatType::Spatial), var(CombatType::Planetary) }
        };
        meta_enum<CombatType>();
    }
    
    {
        static Meta meta = { type<Experience>(), &namspc({}), "Experience", sizeof(Experience), TypeClass::Enum };
        static Enum enu = { type<Experience>(),
            true,
            { "Inexperienced", "LittleExperienced", "Experienced", "Superior", "Elite" },
            { 0, 1, 2, 3, 4 },
            { var(Experience::Inexperienced), var(Experience::LittleExperienced), var(Experience::Experienced), var(Experience::Superior), var(Experience::Elite) }
        };
        meta_enum<Experience>();
    }
    
    {
        static Meta meta = { type<FleetSize>(), &namspc({}), "FleetSize", sizeof(FleetSize), TypeClass::Enum };
        static Enum enu = { type<FleetSize>(),
            true,
            { "Ridicule", "Minuscule", "VerySmall", "Small", "Medium", "Respectable", "Grand", "HighGrand", "Colossal", "Titanesque", "Cyclopean", "Divine" },
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 },
            { var(FleetSize::Ridicule), var(FleetSize::Minuscule), var(FleetSize::VerySmall), var(FleetSize::Small), var(FleetSize::Medium), var(FleetSize::Respectable), var(FleetSize::Grand), var(FleetSize::HighGrand), var(FleetSize::Colossal), var(FleetSize::Titanesque), var(FleetSize::Cyclopean), var(FleetSize::Divine) }
        };
        meta_enum<FleetSize>();
    }
    
    {
        static Meta meta = { type<FleetStance>(), &namspc({}), "FleetStance", sizeof(FleetStance), TypeClass::Enum };
        static Enum enu = { type<FleetStance>(),
            true,
            { "Movement", "SpatialAttack", "SpatialDefense", "PlanetaryAttack", "PlanetaryDefense", "Pillage" },
            { 0, 1, 2, 3, 4, 5 },
            { var(FleetStance::Movement), var(FleetStance::SpatialAttack), var(FleetStance::SpatialDefense), var(FleetStance::PlanetaryAttack), var(FleetStance::PlanetaryDefense), var(FleetStance::Pillage) }
        };
        meta_enum<FleetStance>();
    }
    
    {
        static Meta meta = { type<GameMode>(), &namspc({}), "GameMode", sizeof(GameMode), TypeClass::Enum };
        static Enum enu = { type<GameMode>(),
            true,
            { "Empire", "Tactics", "TurnReport" },
            { 0, 1, 2 },
            { var(GameMode::Empire), var(GameMode::Tactics), var(GameMode::TurnReport) }
        };
        meta_enum<GameMode>();
    }
    
    {
        static Meta meta = { type<Race>(), &namspc({}), "Race", sizeof(Race), TypeClass::Enum };
        static Enum enu = { type<Race>(),
            true,
            { "Human", "Bulvoid", "Techtulon", "Rakrarior", "Seigneur", "Meton", "Tissinar", "Zwiie" },
            { 0, 1, 2, 3, 4, 5, 6, 7 },
            { var(Race::Human), var(Race::Bulvoid), var(Race::Techtulon), var(Race::Rakrarior), var(Race::Seigneur), var(Race::Meton), var(Race::Tissinar), var(Race::Zwiie) }
        };
        meta_enum<Race>();
    }
    
    {
        static Meta meta = { type<Regime>(), &namspc({}), "Regime", sizeof(Regime), TypeClass::Enum };
        static Enum enu = { type<Regime>(),
            true,
            { "Empire", "Dictature", "Democracy", "Theocracy", "Technocracy", "Piratery" },
            { 0, 1, 2, 3, 4, 5 },
            { var(Regime::Empire), var(Regime::Dictature), var(Regime::Democracy), var(Regime::Theocracy), var(Regime::Technocracy), var(Regime::Piratery) }
        };
        meta_enum<Regime>();
    }
    
    {
        static Meta meta = { type<Resource>(), &namspc({}), "Resource", sizeof(Resource), TypeClass::Enum };
        static Enum enu = { type<Resource>(),
            true,
            { "Minerals", "Andrium", "Alcool", "Slaves", "Narcotics", "Food", "Medicine", "Computers", "Plastic", "Robot", "Count" },
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
            { var(Resource::Minerals), var(Resource::Andrium), var(Resource::Alcool), var(Resource::Slaves), var(Resource::Narcotics), var(Resource::Food), var(Resource::Medicine), var(Resource::Computers), var(Resource::Plastic), var(Resource::Robot), var(Resource::Count) }
        };
        meta_enum<Resource>();
    }
    
    {
        static Meta meta = { type<Taxation>(), &namspc({}), "Taxation", sizeof(Taxation), TypeClass::Enum };
        static Enum enu = { type<Taxation>(),
            true,
            { "None", "Light", "Medium", "Heavy", "VeryHeavy", "Total" },
            { 0, 1, 2, 3, 4, 5 },
            { var(Taxation::None), var(Taxation::Light), var(Taxation::Medium), var(Taxation::Heavy), var(Taxation::VeryHeavy), var(Taxation::Total) }
        };
        meta_enum<Taxation>();
    }
    
    {
        static Meta meta = { type<Technology>(), &namspc({}), "Technology", sizeof(Technology), TypeClass::Enum };
        static Enum enu = { type<Technology>(),
            true,
            { "Engines", "EcoEnergy", "Scanners", "PlanetaryShields", "SpatialShields", "LaserPlasmaIons", "Torpedoes", "Bombs", "HullArmor", "Mining", "Terraforming", "Construction", "Piloting", "Commerce", "Personnel", "Administration", "CyberCommand", "CyberCommerce", "CyberDiplomacy", "Count" },
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
            { var(Technology::Engines), var(Technology::EcoEnergy), var(Technology::Scanners), var(Technology::PlanetaryShields), var(Technology::SpatialShields), var(Technology::LaserPlasmaIons), var(Technology::Torpedoes), var(Technology::Bombs), var(Technology::HullArmor), var(Technology::Mining), var(Technology::Terraforming), var(Technology::Construction), var(Technology::Piloting), var(Technology::Commerce), var(Technology::Personnel), var(Technology::Administration), var(Technology::CyberCommand), var(Technology::CyberCommerce), var(Technology::CyberDiplomacy), var(Technology::Count) }
        };
        meta_enum<Technology>();
    }
    
    {
        static Meta meta = { type<WeaponType>(), &namspc({}), "WeaponType", sizeof(WeaponType), TypeClass::Enum };
        static Enum enu = { type<WeaponType>(),
            true,
            { "None", "Ion", "Laser", "Plasma", "Torpedo", "Bomb" },
            { 0, 1, 2, 3, 4, 5 },
            { var(WeaponType::None), var(WeaponType::Ion), var(WeaponType::Laser), var(WeaponType::Plasma), var(WeaponType::Torpedo), var(WeaponType::Bomb) }
        };
        meta_enum<WeaponType>();
    }
    
    
    // Sequences
    {
        static Meta meta = { type<std::vector<CombatFleet>>(), &namspc({}), "std::vector<CombatFleet>", sizeof(std::vector<CombatFleet>), TypeClass::Sequence };
        static Class cls = { type<std::vector<CombatFleet>>() };
        cls.m_content = &type<CombatFleet>();
        meta_sequence<std::vector<CombatFleet>, CombatFleet>();
    }
    
    {
        static Meta meta = { type<std::vector<Commander*>>(), &namspc({}), "std::vector<Commander*>", sizeof(std::vector<Commander*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<Commander*>>() };
        cls.m_content = &type<Commander>();
        meta_sequence<std::vector<Commander*>, Commander*>();
    }
    
    {
        static Meta meta = { type<std::vector<Fleet*>>(), &namspc({}), "std::vector<Fleet*>", sizeof(std::vector<Fleet*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<Fleet*>>() };
        cls.m_content = &type<Fleet>();
        meta_sequence<std::vector<Fleet*>, Fleet*>();
    }
    
    {
        static Meta meta = { type<std::vector<Quadrant*>>(), &namspc({}), "std::vector<Quadrant*>", sizeof(std::vector<Quadrant*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<Quadrant*>>() };
        cls.m_content = &type<Quadrant>();
        meta_sequence<std::vector<Quadrant*>, Quadrant*>();
    }
    
    {
        static Meta meta = { type<std::vector<Star*>>(), &namspc({}), "std::vector<Star*>", sizeof(std::vector<Star*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<Star*>>() };
        cls.m_content = &type<Star>();
        meta_sequence<std::vector<Star*>, Star*>();
    }
    
    
    
    
    
        
    // Combat
    {
        static Meta meta = { type<Combat>(), &namspc({}), "Combat", sizeof(Combat), TypeClass::Struct };
        static Class cls = { type<Combat>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<Combat>(), [](Ref ref, Ref other) { new(&val<Combat>(ref)) Combat(val<Combat>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<Combat>();
    }
    
    
        
    // CombatFleet
    {
        static Meta meta = { type<CombatFleet>(), &namspc({}), "CombatFleet", sizeof(CombatFleet), TypeClass::Struct };
        static Class cls = { type<CombatFleet>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<CombatFleet>(), [](Ref ref, Ref other) { new(&val<CombatFleet>(ref)) CombatFleet(val<CombatFleet>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<CombatFleet>();
    }
    
    
        
    // Commander
    {
        static Meta meta = { type<Commander>(), &namspc({}), "Commander", sizeof(Commander), TypeClass::Object };
        static Class cls = { type<Commander>(),
            // bases
            {  },
            {  },
            // constructors
            {
                { type<Commander>(), [](Ref ref, array<Var> args) { new(&val<Commander>(ref)) Commander( val<mud::Id>(args[0]), val<std::string>(args[1]), val<Race>(args[2]), val<int>(args[3]), val<int>(args[4]), val<int>(args[5]) ); }, { { "id", var(mud::Id()) }, { "name", var(std::string()) }, { "race", var(Race()) }, { "command", var(int()) }, { "commerce", var(int()) }, { "diplomacy", var(int()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Commander>(), member_address(&Commander::m_id), type<mud::Id>(), "id", var(mud::Id()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_race), type<Race>(), "race", var(Race()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_command), type<int>(), "command", var(int()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_commerce), type<int>(), "commerce", var(int()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_diplomacy), type<int>(), "diplomacy", var(int()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_stars), type<std::vector<Star*>>(), "stars", var(std::vector<Star*>()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_fleets), type<std::vector<Fleet*>>(), "fleets", var(std::vector<Fleet*>()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_capital), type<Star>(), "capital", Ref(type<Star>()), Member::Flags(Member::Pointer|Member::Link), nullptr },
                { type<Commander>(), member_address(&Commander::m_regime), type<Regime>(), "regime", var(Regime()), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_power), type<float>(), "power", var(float(0.f)), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_centaures), type<float>(), "centaures", var(float(0.f)), Member::Value, nullptr },
                { type<Commander>(), member_address(&Commander::m_scans), type<Scans>(), "scans", var(Scans()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Commander>(); 
        
        meta_class<Commander>();
    }
    
    
        
    // Construction
    {
        static Meta meta = { type<Construction>(), &namspc({}), "Construction", sizeof(Construction), TypeClass::Struct };
        static Class cls = { type<Construction>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<Construction>(), [](Ref ref, Ref other) { new(&val<Construction>(ref)) Construction(val<Construction>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<Construction>();
    }
    
    
        
    // FleetJump
    {
        static Meta meta = { type<FleetJump>(), &namspc({}), "FleetJump", sizeof(FleetJump), TypeClass::Struct };
        static Class cls = { type<FleetJump>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<FleetJump>(), [](Ref ref, Ref other) { new(&val<FleetJump>(ref)) FleetJump(val<FleetJump>(other)); } }
            },
            // members
            {
                { type<FleetJump>(), member_address(&FleetJump::m_destination), type<mud::uvec2>(), "destination", var(mud::uvec2()), Member::Value, nullptr },
                { type<FleetJump>(), member_address(&FleetJump::m_stance), type<FleetStance>(), "stance", var(FleetStance()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<FleetJump>();
    }
    
    
        
    // FleetSplit
    {
        static Meta meta = { type<FleetSplit>(), &namspc({}), "FleetSplit", sizeof(FleetSplit), TypeClass::Struct };
        static Class cls = { type<FleetSplit>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<FleetSplit>(), [](Ref ref, Ref other) { new(&val<FleetSplit>(ref)) FleetSplit(val<FleetSplit>(other)); } }
            },
            // members
            {
                { type<FleetSplit>(), member_address(&FleetSplit::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<FleetSplit>(), member_address(&FleetSplit::m_stance), type<FleetStance>(), "stance", var(FleetStance()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<FleetSplit>();
    }
    
    
    
    
        
    // PlanetaryCombat
    {
        static Meta meta = { type<PlanetaryCombat>(), &namspc({}), "PlanetaryCombat", sizeof(PlanetaryCombat), TypeClass::Struct };
        static Class cls = { type<PlanetaryCombat>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<PlanetaryCombat>(), [](Ref ref, Ref other) { new(&val<PlanetaryCombat>(ref)) PlanetaryCombat(val<PlanetaryCombat>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<PlanetaryCombat>();
    }
    
    
        
    // Player
    {
        static Meta meta = { type<Player>(), &namspc({}), "Player", sizeof(Player), TypeClass::Object };
        static Class cls = { type<Player>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<Player>();
    }
    
    
    
        
    // Scans
    {
        static Meta meta = { type<Scans>(), &namspc({}), "Scans", sizeof(Scans), TypeClass::Struct };
        static Class cls = { type<Scans>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<Scans>(), [](Ref ref, Ref other) { new(&val<Scans>(ref)) Scans(val<Scans>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<Scans>();
    }
    
    
        
    // Schema
    {
        static Meta meta = { type<Schema>(), &namspc({}), "Schema", sizeof(Schema), TypeClass::Struct };
        static Class cls = { type<Schema>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<Schema>(), [](Ref ref, Ref other) { new(&val<Schema>(ref)) Schema(val<Schema>(other)); } }
            },
            // members
            {
                { type<Schema>(), member_address(&Schema::m_code), type<std::string>(), "code", var(std::string()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_conceptor), type<std::string>(), "conceptor", var(std::string()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_level), type<size_t>(), "level", var(size_t()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_cost), type<float>(), "cost", var(float(0.f)), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_minerals), type<float>(), "minerals", var(float(0.f)), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_andrium), type<float>(), "andrium", var(float(0.f)), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_planetary), type<float>(), "planetary", var(float(0.f)), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_spatial), type<SpatialPower>(), "spatial", var(SpatialPower()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_resistance), type<float>(), "resistance", var(float(0.f)), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_speed), type<size_t>(), "speed", var(size_t()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_scan), type<size_t>(), "scan", var(size_t()), Member::Value, nullptr },
                { type<Schema>(), member_address(&Schema::m_upkeep_factor), type<float>(), "upkeep_factor", var(float(1.f)), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<Schema>();
    }
    
    
    
        
    // SpatialCombat
    {
        static Meta meta = { type<SpatialCombat>(), &namspc({}), "SpatialCombat", sizeof(SpatialCombat), TypeClass::Struct };
        static Class cls = { type<SpatialCombat>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<SpatialCombat>(), [](Ref ref, Ref other) { new(&val<SpatialCombat>(ref)) SpatialCombat(val<SpatialCombat>(other)); } }
            },
            // members
            {
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value, nullptr },
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_allies), type<std::vector<CombatFleet>>(), "allies", var(std::vector<CombatFleet>()), Member::Value, nullptr },
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_enemies), type<std::vector<CombatFleet>>(), "enemies", var(std::vector<CombatFleet>()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<SpatialCombat>();
    }
    
    
        
    // SpatialPower
    {
        static Meta meta = { type<SpatialPower>(), &namspc({}), "SpatialPower", sizeof(SpatialPower), TypeClass::Struct };
        static Class cls = { type<SpatialPower>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<SpatialPower>(), [](Ref ref, Ref other) { new(&val<SpatialPower>(ref)) SpatialPower(val<SpatialPower>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<SpatialPower>();
    }
    
    
        
    // TechDomain
    {
        static Meta meta = { type<TechDomain>(), &namspc({}), "TechDomain", sizeof(TechDomain), TypeClass::Struct };
        static Class cls = { type<TechDomain>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<TechDomain>(), [](Ref ref, Ref other) { new(&val<TechDomain>(ref)) TechDomain(val<TechDomain>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<TechDomain>();
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        
    // Fleet
    {
        static Meta meta = { type<Fleet>(), &namspc({}), "Fleet", sizeof(Fleet), TypeClass::Complex };
        static Class cls = { type<Fleet>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Fleet, mud::Complex>() },
            // constructors
            {
                { type<Fleet>(), [](Ref ref, array<Var> args) { new(&val<Fleet>(ref)) Fleet( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<Commander>(args[3]), val<mud::uvec2>(args[4]), val<std::string>(args[5]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "commander", Ref(type<Commander>()) }, { "coord", var(mud::uvec2()) }, { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Fleet>(), member_address(&Fleet::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_emitter), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::Component, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_receptor), type<toy::Receptor>(), "receptor", Ref(type<toy::Receptor>()), Member::Component, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_active), type<toy::Active>(), "active", Ref(type<toy::Active>()), Member::Component, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Link), nullptr },
                { type<Fleet>(), member_address(&Fleet::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_experience), type<float>(), "experience", var(float(0.f)), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_spatial), type<SpatialPower>(), "spatial", var(SpatialPower()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_spatial_power), type<float>(), "spatial_power", var(float(0.f)), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_planetary_power), type<float>(), "planetary_power", var(float(0.f)), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_speed), type<size_t>(), "speed", var(size_t()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_scan), type<size_t>(), "scan", var(size_t()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_upkeep), type<float>(), "upkeep", var(float(0.f)), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_stance), type<FleetStance>(), "stance", var(FleetStance()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_jump), type<FleetJump>(), "jump", var(FleetJump()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_split), type<FleetSplit>(), "split", var(FleetSplit()), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_fought), type<bool>(), "fought", var(bool(false)), Member::Value, nullptr },
                { type<Fleet>(), member_address(&Fleet::m_ships_updated), type<size_t>(), "ships_updated", var(size_t()), Member::Value, nullptr }
            },
            // methods
            {
                { type<Fleet>(), "order_jump", member_address(&Fleet::order_jump), [](Ref object, array<Var> args, Var& result) { UNUSED(result); val<Fleet>(object).order_jump(val<mud::vec2>(args[0]), val<FleetStance>(args[1])); }, { { "coord", var(mud::vec2()) }, { "stance", var(FleetStance()) } }, Var() },
                { type<Fleet>(), "order_attack", member_address(&Fleet::order_attack), [](Ref object, array<Var> args, Var& result) { UNUSED(result); val<Fleet>(object).order_attack(val<Star>(args[0])); }, { { "star", Ref(type<Star>()) } }, Var() }
            },
            // static members
            {
            }
        };
        
        
        init_pool<Fleet>(); 
        
        meta_class<Fleet>();
    }
    
    
        
    // Galaxy
    {
        static Meta meta = { type<Galaxy>(), &namspc({}), "Galaxy", sizeof(Galaxy), TypeClass::Complex };
        static Class cls = { type<Galaxy>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Galaxy, mud::Complex>() },
            // constructors
            {
                { type<Galaxy>(), [](Ref ref, array<Var> args) { new(&val<Galaxy>(ref)) Galaxy( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "size", var(mud::uvec2()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Galaxy>(), member_address(&Galaxy::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component, nullptr },
                { type<Galaxy>(), member_address(&Galaxy::m_quadrants), type<std::vector<Quadrant*>>(), "quadrants", var(std::vector<Quadrant*>()), Member::Value, nullptr },
                { type<Galaxy>(), member_address(&Galaxy::m_stars), type<std::vector<Star*>>(), "stars", var(std::vector<Star*>()), Member::Value, nullptr },
                { type<Galaxy>(), member_address(&Galaxy::m_fleets), type<std::vector<Fleet*>>(), "fleets", var(std::vector<Fleet*>()), Member::Value, nullptr },
                { type<Galaxy>(), member_address(&Galaxy::m_commanders), type<std::vector<Commander*>>(), "commanders", var(std::vector<Commander*>()), Member::Value, nullptr },
                { type<Galaxy>(), member_address(&Galaxy::m_size), type<mud::uvec2>(), "size", var(mud::uvec2()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Galaxy>(); 
        
        meta_class<Galaxy>();
    }
    
    
        
    // Quadrant
    {
        static Meta meta = { type<Quadrant>(), &namspc({}), "Quadrant", sizeof(Quadrant), TypeClass::Complex };
        static Class cls = { type<Quadrant>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Quadrant, mud::Complex>() },
            // constructors
            {
                { type<Quadrant>(), [](Ref ref, array<Var> args) { new(&val<Quadrant>(ref)) Quadrant( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]), val<float>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "coord", var(mud::uvec2()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Quadrant>(), member_address(&Quadrant::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_emitter), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::Component, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_world_page), type<toy::WorldPage>(), "world_page", Ref(type<toy::WorldPage>()), Member::Component, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_buffer_page), type<toy::BufferPage>(), "buffer_page", Ref(type<toy::BufferPage>()), Member::Component, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_size), type<float>(), "size", var(float()), Member::Value, nullptr },
                { type<Quadrant>(), member_address(&Quadrant::m_stars), type<std::vector<Star*>>(), "stars", var(std::vector<Star*>()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Quadrant>(); 
        
        meta_class<Quadrant>();
    }
    
    
        
    // Star
    {
        static Meta meta = { type<Star>(), &namspc({}), "Star", sizeof(Star), TypeClass::Complex };
        static Class cls = { type<Star>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Star, mud::Complex>() },
            // constructors
            {
                { type<Star>(), [](Ref ref, array<Var> args) { new(&val<Star>(ref)) Star( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]), val<std::string>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "coord", var(mud::uvec2()) }, { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Star>(), member_address(&Star::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component, nullptr },
                { type<Star>(), member_address(&Star::m_emitter), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::Component, nullptr },
                { type<Star>(), member_address(&Star::m_receptor), type<toy::Receptor>(), "receptor", Ref(type<toy::Receptor>()), Member::Component, nullptr },
                { type<Star>(), member_address(&Star::m_active), type<toy::Active>(), "active", Ref(type<toy::Active>()), Member::Component, nullptr },
                { type<Star>(), member_address(&Star::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_stability), type<int>(), "stability", var(int(100)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_revolt), type<bool>(), "revolt", var(bool(false)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_environment), type<int>(), "environment", var(int(10)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_terraformation), type<int>(), "terraformation", var(int(0)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_base_population), type<int>(), "base_population", var(int(0)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_max_population), type<int>(), "max_population", var(int(0)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_population), type<int>(), "population", var(int(0)), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_taxation), type<Taxation>(), "taxation", var(Taxation()), Member::Value, nullptr },
                { type<Star>(), member_address(&Star::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Link), nullptr },
                { type<Star>(), member_address(&Star::m_scan), type<int>(), "scan", var(int(0)), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Star>(); 
        
        meta_class<Star>();
    }
    
    
        
    // Universe
    {
        static Meta meta = { type<Universe>(), &namspc({}), "Universe", sizeof(Universe), TypeClass::Complex };
        static Class cls = { type<Universe>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Universe, mud::Complex>() },
            // constructors
            {
                { type<Universe>(), [](Ref ref, array<Var> args) { new(&val<Universe>(ref)) Universe( val<std::string>(args[0]) ); }, { { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Universe>(), member_address(&Universe::m_world), type<toy::World>(), "world", Ref(type<toy::World>()), Member::Component, nullptr },
                { type<Universe>(), member_address(&Universe::m_bullet_world), type<toy::BulletWorld>(), "bullet_world", Ref(type<toy::BulletWorld>()), Member::Component, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Universe>(); 
        
        meta_class<Universe>();
    }
    
    
        
    // CommanderBrush
    {
        static Meta meta = { type<CommanderBrush>(), &namspc({}), "CommanderBrush", sizeof(CommanderBrush), TypeClass::Object };
        static Class cls = { type<CommanderBrush>(),
            // bases
            { &type<mud::Brush>() },
            { base_offset<CommanderBrush, mud::Brush>() },
            // constructors
            {
            },
            // copy constructor
            {
            },
            // members
            {
                { type<CommanderBrush>(), member_address(&CommanderBrush::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Link), nullptr },
                { type<CommanderBrush>(), member_address(&CommanderBrush::m_radius), type<float>(), "radius", var(float()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<CommanderBrush>();
    }
    
    
        
    // BuildingSchema
    {
        static Meta meta = { type<BuildingSchema>(), &namspc({}), "BuildingSchema", sizeof(BuildingSchema), TypeClass::Struct };
        static Class cls = { type<BuildingSchema>(),
            // bases
            { &type<Schema>() },
            { base_offset<BuildingSchema, Schema>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<BuildingSchema>(), [](Ref ref, Ref other) { new(&val<BuildingSchema>(ref)) BuildingSchema(val<BuildingSchema>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<BuildingSchema>();
    }
    
    
        
    // ShipComponent
    {
        static Meta meta = { type<ShipComponent>(), &namspc({}), "ShipComponent", sizeof(ShipComponent), TypeClass::Struct };
        static Class cls = { type<ShipComponent>(),
            // bases
            { &type<Schema>() },
            { base_offset<ShipComponent, Schema>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<ShipComponent>(), [](Ref ref, Ref other) { new(&val<ShipComponent>(ref)) ShipComponent(val<ShipComponent>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<ShipComponent>();
    }
    
    
        
    // ShipEngine
    {
        static Meta meta = { type<ShipEngine>(), &namspc({}), "ShipEngine", sizeof(ShipEngine), TypeClass::Struct };
        static Class cls = { type<ShipEngine>(),
            // bases
            { &type<Schema>() },
            { base_offset<ShipEngine, Schema>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<ShipEngine>(), [](Ref ref, Ref other) { new(&val<ShipEngine>(ref)) ShipEngine(val<ShipEngine>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<ShipEngine>();
    }
    
    
        
    // ShipHull
    {
        static Meta meta = { type<ShipHull>(), &namspc({}), "ShipHull", sizeof(ShipHull), TypeClass::Struct };
        static Class cls = { type<ShipHull>(),
            // bases
            { &type<Schema>() },
            { base_offset<ShipHull, Schema>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<ShipHull>(), [](Ref ref, Ref other) { new(&val<ShipHull>(ref)) ShipHull(val<ShipHull>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<ShipHull>();
    }
    
    
        
    // ShipSchema
    {
        static Meta meta = { type<ShipSchema>(), &namspc({}), "ShipSchema", sizeof(ShipSchema), TypeClass::Struct };
        static Class cls = { type<ShipSchema>(),
            // bases
            { &type<Schema>() },
            { base_offset<ShipSchema, Schema>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<ShipSchema>(), [](Ref ref, Ref other) { new(&val<ShipSchema>(ref)) ShipSchema(val<ShipSchema>(other)); } }
            },
            // members
            {
                { type<ShipSchema>(), member_address(&ShipSchema::m_size), type<size_t>(), "size", var(size_t()), Member::Value, nullptr },
                { type<ShipSchema>(), member_address(&ShipSchema::m_main_weapon), type<WeaponType>(), "main_weapon", var(WeaponType()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<ShipSchema>();
    }
    

    
        m.m_types.push_back(&type<Combat>());
        m.m_types.push_back(&type<CombatFleet>());
        m.m_types.push_back(&type<CombatType>());
        m.m_types.push_back(&type<Commander>());
        m.m_types.push_back(&type<Construction>());
        m.m_types.push_back(&type<Experience>());
        m.m_types.push_back(&type<FleetJump>());
        m.m_types.push_back(&type<FleetSize>());
        m.m_types.push_back(&type<FleetSplit>());
        m.m_types.push_back(&type<FleetStance>());
        m.m_types.push_back(&type<GameMode>());
        m.m_types.push_back(&type<PlanetaryCombat>());
        m.m_types.push_back(&type<Player>());
        m.m_types.push_back(&type<Race>());
        m.m_types.push_back(&type<Regime>());
        m.m_types.push_back(&type<Resource>());
        m.m_types.push_back(&type<Scans>());
        m.m_types.push_back(&type<Schema>());
        m.m_types.push_back(&type<SpatialCombat>());
        m.m_types.push_back(&type<SpatialPower>());
        m.m_types.push_back(&type<Taxation>());
        m.m_types.push_back(&type<TechDomain>());
        m.m_types.push_back(&type<Technology>());
        m.m_types.push_back(&type<WeaponType>());
        m.m_types.push_back(&type<std::vector<CombatFleet>>());
        m.m_types.push_back(&type<std::vector<Commander*>>());
        m.m_types.push_back(&type<std::vector<Fleet*>>());
        m.m_types.push_back(&type<std::vector<Quadrant*>>());
        m.m_types.push_back(&type<std::vector<Star*>>());
        m.m_types.push_back(&type<Fleet>());
        m.m_types.push_back(&type<Galaxy>());
        m.m_types.push_back(&type<Quadrant>());
        m.m_types.push_back(&type<Star>());
        m.m_types.push_back(&type<Universe>());
        m.m_types.push_back(&type<CommanderBrush>());
        m.m_types.push_back(&type<BuildingSchema>());
        m.m_types.push_back(&type<ShipComponent>());
        m.m_types.push_back(&type<ShipEngine>());
        m.m_types.push_back(&type<ShipHull>());
        m.m_types.push_back(&type<ShipSchema>());
    
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_system(val<toy::Entity>(args[0]), val<mud::uvec3>(args[1]), val<mud::vec3>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "coord", var(mud::uvec3()) }, { "position", var(mud::vec3()) } };
            static Function f = { &namspc({}), "generate_system", function_id<Star*(*)(toy::Entity&, const mud::uvec3&, const mud::vec3&)>(&::generate_system), func, params, Ref(type<Star>()) };
            m.m_functions.push_back(&f);
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_fleet(val<toy::Entity>(args[0]), val<mud::uvec3>(args[1]), val<mud::vec3>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "coord", var(mud::uvec3()) }, { "position", var(mud::vec3()) } };
            static Function f = { &namspc({}), "generate_fleet", function_id<Fleet*(*)(toy::Entity&, const mud::uvec3&, const mud::vec3&)>(&::generate_fleet), func, params, Ref(type<Fleet>()) };
            m.m_functions.push_back(&f);
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_commander(val<toy::Entity>(args[0]), val<Star>(args[1]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "star", Ref(type<Star>()) } };
            static Function f = { &namspc({}), "generate_commander", function_id<Commander*(*)(toy::Entity&, Star&)>(&::generate_commander), func, params, Ref(type<Commander>()) };
            m.m_functions.push_back(&f);
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::assign_system(val<toy::Entity>(args[0]), val<Star>(args[1]), val<std::vector<Commander*>>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "star", Ref(type<Star>()) }, { "commanders", var(std::vector<Commander*>()) } };
            static Function f = { &namspc({}), "assign_system", function_id<Star*(*)(toy::Entity&, Star&, std::vector<Commander*>)>(&::assign_system), func, params, Ref(type<Star>()) };
            m.m_functions.push_back(&f);
        }
    }
}
