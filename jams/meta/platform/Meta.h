

#pragma once

#ifndef MUD_MODULES
#include <meta/platform/Module.h>

#include <obj/Any.h>
#include <obj/Vector.h>
#include <refl/MetaDecl.h>
#include <refl/Module.h>
#endif

namespace mud
{
    void _platform_meta(Module& m)
    {   
    // Base Types
    
    // Enums
    {
        static Meta meta = { type<Faction>(), &namspc({}), "Faction", sizeof(Faction), TypeClass::Enum };
        static Enum enu = { type<Faction>(),
            true,
            { "Ally", "Ennemy" },
            { 0, 1 },
            { var(Faction::Ally), var(Faction::Ennemy) }
        };
        meta_enum<Faction>();
    }
    
    
    // Sequences
    
    
    
    
    
        
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
    
    
    
    
    
    
        
    // Lamp
    {
        static Meta meta = { type<Lamp>(), &namspc({}), "Lamp", sizeof(Lamp), TypeClass::Complex };
        static Class cls = { type<Lamp>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<Lamp, mud::Complex>() },
            // constructors
            {
                { type<Lamp>(), [](Ref ref, array<Var> args) { new(&val<Lamp>(ref)) Lamp( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Lamp>(), member_address(&Lamp::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component },
                { type<Lamp>(), member_address(&Lamp::m_movable), type<toy::Movable>(), "movable", Ref(type<toy::Movable>()), Member::Component }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Lamp>(); 
        
        meta_class<Lamp>();
    }
    
    
        
    // TileWorld
    {
        static Meta meta = { type<TileWorld>(), &namspc({}), "TileWorld", sizeof(TileWorld), TypeClass::Complex };
        static Class cls = { type<TileWorld>(),
            // bases
            { &type<mud::Complex>() },
            { base_offset<TileWorld, mud::Complex>() },
            // constructors
            {
                { type<TileWorld>(), [](Ref ref, array<Var> args) { new(&val<TileWorld>(ref)) TileWorld( val<std::string>(args[0]) ); }, { { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<TileWorld>(), member_address(&TileWorld::m_world), type<toy::World>(), "world", Ref(type<toy::World>()), Member::Component },
                { type<TileWorld>(), member_address(&TileWorld::m_bullet_world), type<toy::BulletWorld>(), "bullet_world", Ref(type<toy::BulletWorld>()), Member::Component },
                { type<TileWorld>(), member_address(&TileWorld::m_navmesh), type<toy::Navmesh>(), "navmesh", Ref(type<toy::Navmesh>()), Member::Component }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<TileWorld>(); 
        
        meta_class<TileWorld>();
    }
    
    
        
    // Crate
    {
        static Meta meta = { type<Crate>(), &namspc({}), "Crate", sizeof(Crate), TypeClass::Complex };
        static Class cls = { type<Crate>(),
            // bases
            { &type<mud::Complex>(), &type<toy::ColliderObject>() },
            { base_offset<Crate, mud::Complex>(), base_offset<Crate, toy::ColliderObject>() },
            // constructors
            {
                { type<Crate>(), [](Ref ref, array<Var> args) { new(&val<Crate>(ref)) Crate( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::vec3>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "extents", var(mud::vec3()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Crate>(), member_address(&Crate::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component },
                { type<Crate>(), member_address(&Crate::m_movable), type<toy::Movable>(), "movable", Ref(type<toy::Movable>()), Member::Component },
                { type<Crate>(), member_address(&Crate::m_extents), type<mud::vec3>(), "extents", var(mud::vec3()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Crate>(); 
        
        meta_class<Crate>();
    }
    
    
        
    // Human
    {
        static Meta meta = { type<Human>(), &namspc({}), "Human", sizeof(Human), TypeClass::Complex };
        static Class cls = { type<Human>(),
            // bases
            { &type<mud::Complex>(), &type<toy::ColliderObject>() },
            { base_offset<Human, mud::Complex>(), base_offset<Human, toy::ColliderObject>() },
            // constructors
            {
                { type<Human>(), [](Ref ref, array<Var> args) { new(&val<Human>(ref)) Human( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<Faction>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "faction", var(Faction()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Human>(), member_address(&Human::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Component },
                { type<Human>(), member_address(&Human::m_emitter), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::Component },
                { type<Human>(), member_address(&Human::m_receptor), type<toy::Receptor>(), "receptor", Ref(type<toy::Receptor>()), Member::Component },
                { type<Human>(), member_address(&Human::m_active), type<toy::Active>(), "active", Ref(type<toy::Active>()), Member::Component }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<Human>(); 
        
        meta_class<Human>();
    }
    

    
        m.m_types.push_back(&type<Faction>());
        m.m_types.push_back(&type<Player>());
        m.m_types.push_back(&type<Lamp>());
        m.m_types.push_back(&type<TileWorld>());
        m.m_types.push_back(&type<Crate>());
        m.m_types.push_back(&type<Human>());
    
    }
}
