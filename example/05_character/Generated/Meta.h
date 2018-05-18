

#pragma once

#include <05_character/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef _05_CHARACTER_REFLECTION_IMPL
    void ex_05_character_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
    
        
    // Walk
    {
        static Meta meta = { type<Walk>(), &namspc({}), "Walk", sizeof(Walk), TypeClass::Object };
        static Class cls = { type<Walk>(),
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
        
        
        
        
        meta_class<Walk>();
    }
    
    
        
    // Human
    {
        static Meta meta = { type<Human>(), &namspc({}), "Human", sizeof(Human), TypeClass::Object };
        static Class cls = { type<Human>(),
            // bases
            { &type<mud::Construct>(), &type<toy::ColliderObject>() },
            { base_offset<Human, mud::Construct>(), base_offset<Human, toy::ColliderObject>() },
            // constructors
            {
                { type<Human>(), [](Ref ref, array<Var> args) { new(&val<Human>(ref)) Human( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<float>(args[3]), val<float>(args[4]), val<std::string>(args[5]), val<std::string>(args[6]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "radius", var(float()) }, { "height", var(float()) }, { "first_name", var(std::string()) }, { "last_name", var(std::string()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<Human>(), member_address(&Human::m_first_name), type<std::string>(), "first_name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Human>(), member_address(&Human::m_last_name), type<std::string>(), "last_name", var(std::string()), Member::Flags(Member::Value|Member::Mutable) },
                { type<Human>(), member_address(&Human::m_counter), type<float>(), "counter", var(float(0.f)), Member::Flags(Member::Value|Member::Mutable) },
                { type<Human>(), member_address(&Human::m_next_change), type<float>(), "next_change", var(float()), Member::Flags(Member::Value|Member::Mutable) }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<std::string>();
        
        init_pool<Human>(); 
        
        meta_class<Human>();
    }
    

    
        module.m_types.push_back(&type<Walk>());
        module.m_types.push_back(&type<Human>());
    
    }
#endif

}
