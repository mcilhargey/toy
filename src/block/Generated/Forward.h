

#pragma once

#include <obj/Config.h>

#include <obj/Generated/Forward.h>
#include <math/Generated/Forward.h>
#include <geom/Generated/Forward.h>
#include <util/Generated/Forward.h>
#include <core/Generated/Forward.h>
#include <ui/Generated/Forward.h>
#include <uio/Generated/Forward.h>
#include <gfx/Generated/Forward.h>
#include <visu/Generated/Forward.h>
#include <edit/Generated/Forward.h>

#if defined TOY_BLOCK_LIB
#define TOY_BLOCK_EXPORT MUD_EXPORT
#else
#define TOY_BLOCK_EXPORT MUD_IMPORT
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

    enum MatterState : unsigned int;
    
    
    struct Hunk;
    class Block;
    class OBlock;
    class Chunk;
    class Element;
    class Heap;
    class ChunkFilter;
    class GroundChunk;
    class Earth;
    class Stone;
    class Sand;
    class Air;
    class Gas;
    class Minerals;
    class Fungus;
    class Water;
    class Sector;
    struct BlockState;
}

namespace bx {

    
    
}

