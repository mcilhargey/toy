

#pragma once

#include <shell/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef TOY_SHELL_REFLECTION_IMPL
    void shell_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
    
    
        
    // toy::GameShell
    {
        static Meta meta = { type<toy::GameShell>(), &namspc({ "toy" }), "GameShell", sizeof(toy::GameShell), TypeClass::Object };
        static Class cls = { type<toy::GameShell>(),
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
        
        
        
        
        meta_class<toy::GameShell>();
    }
    
    

    
        module.m_types.push_back(&type<toy::GameShell>());
    
    }
#endif

}
