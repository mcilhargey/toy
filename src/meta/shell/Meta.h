

#pragma once

#ifndef MUD_MODULES
#include <meta/shell/Module.h>

#include <obj/Any.h>
#include <obj/Vector.h>
#include <refl/MetaDecl.h>
#include <refl/Module.h>
#endif

namespace mud
{
    void toy_shell_meta(Module& m)
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
    
    

    
        m.m_types.push_back(&type<toy::GameShell>());
    
    }
}
