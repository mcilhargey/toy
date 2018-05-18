

#pragma once

#include <visu/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef TOY_VISU_REFLECTION_IMPL
    void visu_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    
    // Sequences
    
    
    
    
    
        
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
                { type<toy::SoundSource>(), Address(), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None }
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
                { type<toy::VisuScene>(), Address(), type<toy::Vision>(), "vision", Ref(type<toy::Vision>()), Member::None },
                { type<toy::VisuScene>(), Address(), type<toy::World>(), "world", Ref(type<toy::World>()), Member::None },
                { type<toy::VisuScene>(), Address(), type<toy::VisuSystem>(), "visu_system", Ref(type<toy::VisuSystem>()), Member::None },
                { type<toy::VisuScene>(), Address(), type<mud::GfxSystem>(), "gfx_system", Ref(type<mud::GfxSystem>()), Member::None },
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
    
    

    
        module.m_types.push_back(&type<toy::SoundSource>());
        module.m_types.push_back(&type<toy::VisuScene>());
        module.m_types.push_back(&type<toy::VisuSystem>());
        module.m_types.push_back(&type<toy::VisuModule>());
    
    }
#endif

}
