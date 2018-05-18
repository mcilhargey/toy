

#pragma once

#include <obj/Config.h>

#include <obj/Generated/Forward.h>
#include <math/Generated/Forward.h>
#include <geom/Generated/Forward.h>
#include <util/Generated/Forward.h>
#include <core/Generated/Forward.h>
#include <ui/Generated/Forward.h>
#include <gfx/Generated/Forward.h>
#include <snd/Generated/Forward.h>

#if defined TOY_VISU_LIB
#define TOY_VISU_EXPORT MUD_EXPORT
#else
#define TOY_VISU_EXPORT MUD_IMPORT
#endif


    
    

namespace std {

    
    
}

namespace mud {
namespace ui {

    
    
}
}

namespace glm {

    
    
}

namespace bgfx {

    
    
}

namespace mud {
namespace gfx {

    
    
}
}

namespace mud {

    
    
}

namespace mud {
namespace detail {

    
    
}
}

namespace bimg {

    
    
}

namespace toy {

    
    
    class VisuModule;
    struct VisuPainter;
    struct EntityPainter;
    struct VisuCamera;
    struct SceneState;
    class VisuScene;
    class VisuSystem;
    class OgreViewport;
    class SoundSource;
}

namespace bx {

    
    
}

