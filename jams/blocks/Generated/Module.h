
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
#include <util/Generated/Module.h>
#include <core/Generated/Module.h>
#include <db/Generated/Module.h>
#include <ui/Generated/Module.h>
#include <uio/Generated/Module.h>
#include <gfx/Generated/Module.h>
#include <visu/Generated/Module.h>
#include <edit/Generated/Module.h>
#include <block/Generated/Module.h>
#include <gen/Generated/Module.h>
#include <shell/Generated/Module.h>
#include <05_character/Generated/Module.h>
        
#include <blocks/Generated/Forward.h>
#include <blocks/Generated/Types.h>

#include <blocks/ex_blocks.h>
#include <blocks/Generator.h>
#include <blocks/Scene.h>
#include <blocks/Ui.h>

	class _EX_BLOCKS_EXPORT blocks : public Module
	{
	private:
		blocks();

	public:
		static blocks& module() { static blocks instance; return instance; }
	};

#ifdef _BLOCKS_MODULE
extern "C"
_EX_BLOCKS_EXPORT Module& getModule();
#endif
