
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
        
#include <space/Generated/Forward.h>
#include <space/Generated/Types.h>

#include <space/Dice.h>
#include <space/ex_space.h>
#include <space/Generator.h>
#include <space/Scene.h>
#include <space/Ui.h>

	class _EX_SPACE_EXPORT space : public Module
	{
	private:
		space();

	public:
		static space& module() { static space instance; return instance; }
	};

#ifdef _SPACE_MODULE
extern "C"
_EX_SPACE_EXPORT Module& getModule();
#endif
