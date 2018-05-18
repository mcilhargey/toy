

#pragma once

#include <platform/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef _EX_PLATFORM_REFLECTION_IMPL
    void platform_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
        
    // Worldblock
    {
        static Meta meta = { type<Worldblock>(), &namspc({}), "Worldblock", sizeof(Worldblock), TypeClass::Object };
        static Class cls = { type<Worldblock>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<Worldblock, mud::Construct>() },
            // constructors
            {
                { type<Worldblock>(), [](Ref ref, array<Var> args) { new(&val<Worldblock>(ref)) Worldblock( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec3>(args[3]), val<mud::vec3>(args[4]), val<mud::WaveTileset>(args[5]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "size", var(mud::uvec3()) }, { "period", var(mud::vec3()) }, { "tileset", var(mud::WaveTileset()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Worldblock>(), member_address(&Worldblock::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Worldblock>(), member_address(&Worldblock::m_emitter), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::None },
                { type<Worldblock>(), member_address(&Worldblock::m_world_page), type<toy::WorldPage>(), "world_page", Ref(type<toy::WorldPage>()), Member::None },
                { type<Worldblock>(), member_address(&Worldblock::m_navblock), type<toy::Navblock>(), "navblock", Ref(type<toy::Navblock>()), Member::None },
                { type<Worldblock>(), member_address(&Worldblock::m_tileblock), type<mud::Tileblock>(), "tileblock", Ref(type<mud::Tileblock>()), Member::None }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Worldblock>(); 
        
        meta_class<Worldblock>();
    }
    
    
        
    // TileWorld
    {
        static Meta meta = { type<TileWorld>(), &namspc({}), "TileWorld", sizeof(TileWorld), TypeClass::Object };
        static Class cls = { type<TileWorld>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<TileWorld, mud::Construct>() },
            // constructors
            {
                { type<TileWorld>(), [](Ref ref, array<Var> args) { new(&val<TileWorld>(ref)) TileWorld( val<std::string>(args[0]) ); }, { { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<TileWorld>(), member_address(&TileWorld::m_world), type<toy::World>(), "world", Ref(type<toy::World>()), Member::None },
                { type<TileWorld>(), member_address(&TileWorld::m_bulletWorld), type<toy::BulletWorld>(), "bulletWorld", Ref(type<toy::BulletWorld>()), Member::None },
                { type<TileWorld>(), member_address(&TileWorld::m_navmesh), type<toy::Navmesh>(), "navmesh", Ref(type<toy::Navmesh>()), Member::None }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::World>();
        
        init_pool<TileWorld>(); 
        
        meta_class<TileWorld>();
    }
    
    
        
    // Human2
    {
        static Meta meta = { type<Human2>(), &namspc({}), "Human2", sizeof(Human2), TypeClass::Object };
        static Class cls = { type<Human2>(),
            // bases
            { &type<mud::Construct>(), &type<toy::ColliderObject>() },
            { base_offset<Human2, mud::Construct>(), base_offset<Human2, toy::ColliderObject>() },
            // constructors
            {
                { type<Human2>(), [](Ref ref, array<Var> args) { new(&val<Human2>(ref)) Human2( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<std::string>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Human2>(), member_address(&Human2::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Human2>(), member_address(&Human2::m_name), type<std::string>(), "name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Human2>(); 
        
        meta_class<Human2>();
    }
    

    
        module.m_types.push_back(&type<Worldblock>());
        module.m_types.push_back(&type<TileWorld>());
        module.m_types.push_back(&type<Human2>());
    
    }
#endif

}
