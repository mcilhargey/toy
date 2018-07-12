

#pragma once

#ifndef MUD_MODULES
#include <meta/visu/Module.h>

#include <obj/Any.h>
#include <obj/Vector.h>
#include <refl/MetaDecl.h>
#include <refl/Module.h>
#endif

namespace mud
{
    void toy_visu_meta(Module& m)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
        
    // toy::PhysicDebugDraw
    {
        static Meta meta = { type<toy::PhysicDebugDraw>(), &namspc({ "toy" }), "PhysicDebugDraw", sizeof(toy::PhysicDebugDraw), TypeClass::Object };
        static Class cls = { type<toy::PhysicDebugDraw>(),
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
        
        
        
        
        meta_class<toy::PhysicDebugDraw>();
    }
    
    
        
    // toy::SoundSource
    {
        static Meta meta = { type<toy::SoundSource>(), &namspc({ "toy" }), "SoundSource", sizeof(toy::SoundSource), TypeClass::Object };
        static Class cls = { type<toy::SoundSource>(),
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
                { type<toy::SoundSource>(), Address(), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::Link }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<toy::SoundSource>();
    }
    
    
    
    
    
    
    
    
        
    // toy::VisuScene
    {
        static Meta meta = { type<toy::VisuScene>(), &namspc({ "toy" }), "VisuScene", sizeof(toy::VisuScene), TypeClass::Object };
        static Class cls = { type<toy::VisuScene>(),
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
                { type<toy::VisuScene>(), Address(), type<toy::World>(), "world", Ref(type<toy::World>()), Member::Link },
                { type<toy::VisuScene>(), Address(), type<toy::VisuSystem>(), "visu_system", Ref(type<toy::VisuSystem>()), Member::Link },
                { type<toy::VisuScene>(), Address(), type<mud::GfxSystem>(), "gfx_system", Ref(type<mud::GfxSystem>()), Member::Link },
                { type<toy::VisuScene>(), member_address(&toy::VisuScene::m_scene), type<mud::Scene>(), "scene", Ref(type<mud::Scene>()), Member::None }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<toy::VisuScene>();
    }
    
    
        
    // toy::VisuSystem
    {
        static Meta meta = { type<toy::VisuSystem>(), &namspc({ "toy" }), "VisuSystem", sizeof(toy::VisuSystem), TypeClass::Object };
        static Class cls = { type<toy::VisuSystem>(),
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
        
        
        
        
        meta_class<toy::VisuSystem>();
    }
    
    
        
    // toy::VisuModule
    {
        static Meta meta = { type<toy::VisuModule>(), &namspc({ "toy" }), "VisuModule", sizeof(toy::VisuModule), TypeClass::Object };
        static Class cls = { type<toy::VisuModule>(),
            // bases
            { &type<mud::Module>() },
            { base_offset<toy::VisuModule, mud::Module>() },
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
        
        
        
        
        meta_class<toy::VisuModule>();
    }
    
    

    
        m.m_types.push_back(&type<toy::PhysicDebugDraw>());
        m.m_types.push_back(&type<toy::SoundSource>());
        m.m_types.push_back(&type<toy::VisuScene>());
        m.m_types.push_back(&type<toy::VisuSystem>());
        m.m_types.push_back(&type<toy::VisuModule>());
    
    }
}
