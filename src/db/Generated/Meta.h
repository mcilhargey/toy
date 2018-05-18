

#pragma once

#include <db/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef TOY_DB_REFLECTION_IMPL
    void db_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
        
    // toy::SqliteDatabase
    {
        static Meta meta = { type<toy::SqliteDatabase>(), &namspc({ "toy" }), "SqliteDatabase", sizeof(toy::SqliteDatabase), TypeClass::Object };
        static Class cls = { type<toy::SqliteDatabase>(),
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
        
        
        
        
        meta_class<toy::SqliteDatabase>();
    }
    
    
    
    
    

    
        module.m_types.push_back(&type<toy::SqliteDatabase>());
    
    }
#endif

}
