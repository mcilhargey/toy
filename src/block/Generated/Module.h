
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
#include <geom/Generated/Module.h>
#include <util/Generated/Module.h>
#include <core/Generated/Module.h>
#include <ui/Generated/Module.h>
#include <uio/Generated/Module.h>
#include <gfx/Generated/Module.h>
#include <visu/Generated/Module.h>
#include <edit/Generated/Module.h>
        
#include <block/Generated/Forward.h>
#include <block/Generated/Types.h>

#include <block/Block.h>
#include <block/Chunk.h>
#include <block/Element.h>
#include <block/Elements.h>
#include <block/Sector.h>
#include <block/VisuBlock.h>

namespace toy
{
	class TOY_BLOCK_EXPORT toyblock : public Module
	{
	private:
		toyblock();

	public:
		static toyblock& module() { static toyblock instance; return instance; }
	};
}

#ifdef TOY_BLOCK_MODULE
extern "C"
TOY_BLOCK_EXPORT Module& getModule();
#endif
