

#pragma once

#include <blocks/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef _EX_BLOCKS_REFLECTION_IMPL
    void blocks_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    {
        static Meta meta = { type<std::vector<toy::Element*>>(), &namspc({}), "std::vector<toy::Element*>", sizeof(std::vector<toy::Element*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<toy::Element*>>() };
        cls.m_content = &type<toy::Element>();
        meta_sequence<std::vector<toy::Element*>, toy::Element*>();
    }
    
    
    
    
        
    // BlockWorld
    {
        static Meta meta = { type<BlockWorld>(), &namspc({}), "BlockWorld", sizeof(BlockWorld), TypeClass::Object };
        static Class cls = { type<BlockWorld>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<BlockWorld, mud::Construct>() },
            // constructors
            {
                { type<BlockWorld>(), [](Ref ref, array<Var> args) { new(&val<BlockWorld>(ref)) BlockWorld( val<std::string>(args[0]) ); }, { { "name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<BlockWorld>(), member_address(&BlockWorld::m_world), type<toy::World>(), "world", Ref(type<toy::World>()), Member::None },
                { type<BlockWorld>(), member_address(&BlockWorld::m_sectors), type<std::vector<toy::Sector*>>(), "sectors", var(std::vector<toy::Sector*>()), Member::Flags(Member::Value|Member::Mutable) },
                { type<BlockWorld>(), member_address(&BlockWorld::m_grid), type<mud::Grid<toy::Block*>>(), "grid", var(mud::Grid<toy::Block*>()), Member::Flags(Member::Value|Member::Mutable) },
                { type<BlockWorld>(), member_address(&BlockWorld::m_elements), type<std::vector<toy::Element*>>(), "elements", var(std::vector<toy::Element*>()), Member::Flags(Member::Value|Member::Mutable) }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::World>();
        
        init_pool<BlockWorld>(); 
        
        meta_class<BlockWorld>();
    }
    
    
        
    // Crate
    {
        static Meta meta = { type<Crate>(), &namspc({}), "Crate", sizeof(Crate), TypeClass::Object };
        static Class cls = { type<Crate>(),
            // bases
            { &type<mud::Construct>(), &type<toy::ColliderObject>() },
            { base_offset<Crate, mud::Construct>(), base_offset<Crate, toy::ColliderObject>() },
            // constructors
            {
                { type<Crate>(), [](Ref ref, array<Var> args) { new(&val<Crate>(ref)) Crate( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::vec3>(args[3]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "extents", var(mud::vec3()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Crate>(), member_address(&Crate::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<Crate>(), member_address(&Crate::m_extents), type<mud::vec3>(), "extents", var(mud::vec3()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<Crate>(); 
        
        meta_class<Crate>();
    }
    

    
        module.m_types.push_back(&type<std::vector<toy::Element*>>());
        module.m_types.push_back(&type<BlockWorld>());
        module.m_types.push_back(&type<Crate>());
    
        {
            auto func = [](array<Var> args, Var& result) { UNUSED(result);  ::paint_block_height(val<toy::Block>(args[0]), val<mud::Image256>(args[1])); };
            std::vector<Param> params = { { "block", Ref(type<toy::Block>()) }, { "image", var(mud::Image256()) } };
            module.m_functions.push_back({ &namspc({}), "paint_block_height", function_id<void(*)(toy::Block&, mud::Image256&)>(&::paint_block_height), func, params, Var() });
        }
        {
            auto func = [](array<Var> args, Var& result) { UNUSED(result);  ::paint_block_elements(val<toy::Block>(args[0]), val<mud::Image256>(args[1])); };
            std::vector<Param> params = { { "block", Ref(type<toy::Block>()) }, { "image", var(mud::Image256()) } };
            module.m_functions.push_back({ &namspc({}), "paint_block_elements", function_id<void(*)(toy::Block&, mud::Image256&)>(&::paint_block_elements), func, params, Var() });
        }
    }
#endif

}
