

#pragma once

#include <space/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    template <> _EX_SPACE_EXPORT Type& type<FleetJump::State>();
    template <> _EX_SPACE_EXPORT Type& type<SpatialCombat::State>();
    
#ifdef _EX_SPACE_REFLECTION_IMPL
    void space_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    {
        static Meta meta = { type<FleetStance>(), &namspc({}), "FleetStance", sizeof(FleetStance), TypeClass::Enum };
        static Enum enu = { type<FleetStance>(),
            true,
            { "Movement", "SpatialAttack", "SpatialDefense", "PlanetaryAttack", "PlanetaryDefense" },
            { 0, 1, 2, 3, 4 },
            { var(FleetStance::Movement), var(FleetStance::SpatialAttack), var(FleetStance::SpatialDefense), var(FleetStance::PlanetaryAttack), var(FleetStance::PlanetaryDefense) }
        };
        meta_enum<FleetStance>();
    }
    
    {
        static Meta meta = { type<WeaponType>(), &namspc({}), "WeaponType", sizeof(WeaponType), TypeClass::Enum };
        static Enum enu = { type<WeaponType>(),
            true,
            { "Ion", "Laser", "Plasma", "Torpedo", "Bomb" },
            { 0, 1, 2, 3, 4 },
            { var(WeaponType::Ion), var(WeaponType::Laser), var(WeaponType::Plasma), var(WeaponType::Torpedo), var(WeaponType::Bomb) }
        };
        meta_enum<WeaponType>();
    }
    
    
    // Sequences
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
    
    
    
    
        
    // Commander
    {
        static Meta meta = { type<Commander>(), &namspc({}), "Commander", sizeof(Commander), TypeClass::Object };
        static Class cls = { type<Commander>(),
            // bases
            {  },
            {  },
            // constructors
            {
                { type<Commander>(), [](Ref ref, array<Var> args) { new(&val<Commander>(ref)) Commander( val<mud::Id>(args[0]), val<std::string>(args[1]), val<int>(args[2]), val<int>(args[3]), val<int>(args[4]) ); }, { { "id", var(mud::Id()) }, { "name", var(std::string()) }, { "command", var(int()) }, { "commerce", var(int()) }, { "diplomacy", var(int()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Commander>(), member_address(&Commander::m_id), type<mud::Id>(), "id", var(mud::Id()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Commander>(), member_address(&Commander::m_name), type<std::string>(), "name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Commander>(), member_address(&Commander::m_command), type<int>(), "command", var(int()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Commander>(), member_address(&Commander::m_commerce), type<int>(), "commerce", var(int()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Commander>(), member_address(&Commander::m_diplomacy), type<int>(), "diplomacy", var(int()), Member::Flags(Member::Value|Member::Mutable) }
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
                { type<FleetJump>(), member_address(&FleetJump::m_destination), type<mud::uvec2>(), "destination", var(mud::uvec2()), Member::Value }
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
    
    
    
        
    // ShipComponent
    {
        static Meta meta = { type<ShipComponent>(), &namspc({}), "ShipComponent", sizeof(ShipComponent), TypeClass::Object };
        static Class cls = { type<ShipComponent>(),
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
        
        
        
        
        meta_class<ShipComponent>();
    }
    
    
    
        
    // ShipEngine
    {
        static Meta meta = { type<ShipEngine>(), &namspc({}), "ShipEngine", sizeof(ShipEngine), TypeClass::Object };
        static Class cls = { type<ShipEngine>(),
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
        
        
        
        
        meta_class<ShipEngine>();
    }
    
    
        
    // ShipHull
    {
        static Meta meta = { type<ShipHull>(), &namspc({}), "ShipHull", sizeof(ShipHull), TypeClass::Object };
        static Class cls = { type<ShipHull>(),
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
        
        
        
        
        meta_class<ShipHull>();
    }
    
    
        
    // ShipSchema
    {
        static Meta meta = { type<ShipSchema>(), &namspc({}), "ShipSchema", sizeof(ShipSchema), TypeClass::Struct };
        static Class cls = { type<ShipSchema>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<ShipSchema>(), [](Ref ref, Ref other) { new(&val<ShipSchema>(ref)) ShipSchema(val<ShipSchema>(other)); } }
            },
            // members
            {
                { type<ShipSchema>(), member_address(&ShipSchema::m_size), type<size_t>(), "size", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_code), type<std::string>(), "code", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_name), type<std::string>(), "name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_conceptor), type<std::string>(), "conceptor", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_level), type<size_t>(), "level", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_cost), type<float>(), "cost", var(float()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_minerals), type<float>(), "minerals", var(float()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_andrium), type<float>(), "andrium", var(float()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_planetary), type<float>(), "planetary", var(float()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_resistance), type<float>(), "resistance", var(float()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_speed), type<size_t>(), "speed", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_scan), type<size_t>(), "scan", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_weapon_count), type<mud::uvec4>(), "weapon_count", var(mud::uvec4()), Member::Flags(Member::Value|Member::Mutable) },
                { type<ShipSchema>(), member_address(&ShipSchema::m_weapon_ratio), type<mud::vec4>(), "weapon_ratio", var(mud::vec4()), Member::Flags(Member::Value|Member::Mutable) }
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
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value },
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_flotilla_0), type<std::vector<Fleet*>>(), "flotilla_0", var(std::vector<Fleet*>()), Member::Value },
                { type<SpatialCombat>(), member_address(&SpatialCombat::m_flotilla_1), type<std::vector<Fleet*>>(), "flotilla_1", var(std::vector<Fleet*>()), Member::Value }
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
                { type<CommanderBrush>(), member_address(&CommanderBrush::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Mutable|Member::Link) },
                { type<CommanderBrush>(), member_address(&CommanderBrush::m_radius), type<float>(), "radius", var(float()), Member::Flags(Member::Value|Member::Mutable) }
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
    
    
        
    // Star
    {
        static Meta meta = { type<Star>(), &namspc({}), "Star", sizeof(Star), TypeClass::Object };
        static Class cls = { type<Star>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Star, mud::Construct>() },
            // constructors
            {
                { type<Star>(), [](Ref ref, array<Var> args) { new(&val<Star>(ref)) Star( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]), val<std::string>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "coord", var(mud::uvec2()) }, { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Star>(), member_address(&Star::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Star>(), member_address(&Star::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Star>(), member_address(&Star::m_name), type<std::string>(), "name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Star>(), member_address(&Star::m_minerals), type<int>(), "minerals", var(int(0)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Star>(), member_address(&Star::m_andrium), type<int>(), "andrium", var(int(0)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Star>(), member_address(&Star::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Mutable|Member::Link) }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Star>(); 
        
        meta_class<Star>();
    }
    
    
        
    // Fleet
    {
        static Meta meta = { type<Fleet>(), &namspc({}), "Fleet", sizeof(Fleet), TypeClass::Object };
        static Class cls = { type<Fleet>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Fleet, mud::Construct>() },
            // constructors
            {
                { type<Fleet>(), [](Ref ref, array<Var> args) { new(&val<Fleet>(ref)) Fleet( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<Commander>(args[3]), val<mud::uvec2>(args[4]), val<std::string>(args[5]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "commander", Ref(type<Commander>()) }, { "coord", var(mud::uvec2()) }, { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Fleet>(), member_address(&Fleet::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Fleet>(), member_address(&Fleet::m_commander), type<Commander>(), "commander", Ref(type<Commander>()), Member::Flags(Member::Pointer|Member::Mutable|Member::Link) },
                { type<Fleet>(), member_address(&Fleet::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_name), type<std::string>(), "name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_experience), type<float>(), "experience", var(float(0.f)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_spatial_power), type<float>(), "spatial_power", var(float(0.f)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_planetary), type<float>(), "planetary", var(float(0.f)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_weapon_ratio), type<mud::vec4>(), "weapon_ratio", var(mud::vec4()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_speed), type<size_t>(), "speed", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_scan), type<size_t>(), "scan", var(size_t()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_stance), type<FleetStance>(), "stance", var(FleetStance()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_jump), type<FleetJump>(), "jump", var(FleetJump()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Fleet>(), member_address(&Fleet::m_composition_updated), type<size_t>(), "composition_updated", var(size_t()), Member::Value }
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
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Fleet>(); 
        
        meta_class<Fleet>();
    }
    
    
        
    // Quadrant
    {
        static Meta meta = { type<Quadrant>(), &namspc({}), "Quadrant", sizeof(Quadrant), TypeClass::Object };
        static Class cls = { type<Quadrant>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Quadrant, mud::Construct>() },
            // constructors
            {
                { type<Quadrant>(), [](Ref ref, array<Var> args) { new(&val<Quadrant>(ref)) Quadrant( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]), val<float>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "coord", var(mud::uvec2()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Quadrant>(), member_address(&Quadrant::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Quadrant>(), member_address(&Quadrant::m_coord), type<mud::uvec2>(), "coord", var(mud::uvec2()), Member::Value },
                { type<Quadrant>(), member_address(&Quadrant::m_size), type<float>(), "size", var(float()), Member::Value },
                { type<Quadrant>(), member_address(&Quadrant::m_stars), type<std::vector<Star*>>(), "stars", var(std::vector<Star*>()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Quadrant>(); 
        
        meta_class<Quadrant>();
    }
    
    
        
    // Galaxy
    {
        static Meta meta = { type<Galaxy>(), &namspc({}), "Galaxy", sizeof(Galaxy), TypeClass::Object };
        static Class cls = { type<Galaxy>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Galaxy, mud::Construct>() },
            // constructors
            {
                { type<Galaxy>(), [](Ref ref, array<Var> args) { new(&val<Galaxy>(ref)) Galaxy( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec2>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "size", var(mud::uvec2()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Galaxy>(), member_address(&Galaxy::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Galaxy>(), member_address(&Galaxy::m_quadrants), type<std::vector<Quadrant*>>(), "quadrants", var(std::vector<Quadrant*>()), Member::Value },
                { type<Galaxy>(), member_address(&Galaxy::m_stars), type<std::vector<Star*>>(), "stars", var(std::vector<Star*>()), Member::Value },
                { type<Galaxy>(), member_address(&Galaxy::m_fleets), type<std::vector<Fleet*>>(), "fleets", var(std::vector<Fleet*>()), Member::Value },
                { type<Galaxy>(), member_address(&Galaxy::m_commanders), type<std::vector<Commander*>>(), "commanders", var(std::vector<Commander*>()), Member::Value },
                { type<Galaxy>(), member_address(&Galaxy::m_size), type<mud::uvec2>(), "size", var(mud::uvec2()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Galaxy>(); 
        
        meta_class<Galaxy>();
    }
    
    
        
    // Universe
    {
        static Meta meta = { type<Universe>(), &namspc({}), "Universe", sizeof(Universe), TypeClass::Object };
        static Class cls = { type<Universe>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Universe, mud::Construct>() },
            // constructors
            {
                { type<Universe>(), [](Ref ref, array<Var> args) { new(&val<Universe>(ref)) Universe( val<std::string>(args[0]) ); }, { { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Universe>(), member_address(&Universe::m_world), type<toy::World>(), "world", Ref(type<toy::World>()), Member::None }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::World>();
        
        init_pool<Universe>(); 
        
        meta_class<Universe>();
    }
    

    
        module.m_types.push_back(&type<Commander>());
        module.m_types.push_back(&type<FleetJump>());
        module.m_types.push_back(&type<FleetStance>());
        module.m_types.push_back(&type<ShipComponent>());
        module.m_types.push_back(&type<ShipEngine>());
        module.m_types.push_back(&type<ShipHull>());
        module.m_types.push_back(&type<ShipSchema>());
        module.m_types.push_back(&type<SpatialCombat>());
        module.m_types.push_back(&type<SpatialPower>());
        module.m_types.push_back(&type<WeaponType>());
        module.m_types.push_back(&type<std::vector<Commander*>>());
        module.m_types.push_back(&type<std::vector<Fleet*>>());
        module.m_types.push_back(&type<std::vector<Quadrant*>>());
        module.m_types.push_back(&type<std::vector<Star*>>());
        module.m_types.push_back(&type<CommanderBrush>());
        module.m_types.push_back(&type<Star>());
        module.m_types.push_back(&type<Fleet>());
        module.m_types.push_back(&type<Quadrant>());
        module.m_types.push_back(&type<Galaxy>());
        module.m_types.push_back(&type<Universe>());
    
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_system(val<toy::Entity>(args[0]), val<mud::uvec3>(args[1]), val<mud::vec3>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "coord", var(mud::uvec3()) }, { "position", var(mud::vec3()) } };
            module.m_functions.push_back({ &namspc({}), "generate_system", function_id<Star*(*)(toy::Entity&, const mud::uvec3&, const mud::vec3&)>(&::generate_system), func, params, Ref(type<Star>()) });
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_fleet(val<toy::Entity>(args[0]), val<mud::uvec3>(args[1]), val<mud::vec3>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "coord", var(mud::uvec3()) }, { "position", var(mud::vec3()) } };
            module.m_functions.push_back({ &namspc({}), "generate_fleet", function_id<Fleet*(*)(toy::Entity&, const mud::uvec3&, const mud::vec3&)>(&::generate_fleet), func, params, Ref(type<Fleet>()) });
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::generate_commander(val<toy::Entity>(args[0]), val<Star>(args[1]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "star", Ref(type<Star>()) } };
            module.m_functions.push_back({ &namspc({}), "generate_commander", function_id<Commander*(*)(toy::Entity&, Star&)>(&::generate_commander), func, params, Ref(type<Commander>()) });
        }
        {
            auto func = [](array<Var> args, Var& result) {  result = Ref(::assign_system(val<toy::Entity>(args[0]), val<Star>(args[1]), val<std::vector<Commander*>>(args[2]))); };
            std::vector<Param> params = { { "origin", Ref(type<toy::Entity>()) }, { "star", Ref(type<Star>()) }, { "commanders", var(std::vector<Commander*>()) } };
            module.m_functions.push_back({ &namspc({}), "assign_system", function_id<Star*(*)(toy::Entity&, Star&, std::vector<Commander*>)>(&::assign_system), func, params, Ref(type<Star>()) });
        }
    }
#endif

}
