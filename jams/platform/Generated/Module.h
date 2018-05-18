
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
        
#include <platform/Generated/Forward.h>
#include <platform/Generated/Types.h>

#include <platform/ex_platform.h>
#include <platform/Generator.h>
#include <platform/Scene.h>
#include <platform/Ui.h>

	class _EX_PLATFORM_EXPORT platform : public Module
	{
	private:
		platform();

	public:
		static platform& module() { static platform instance; return instance; }
	};

#ifdef _PLATFORM_MODULE
extern "C"
_EX_PLATFORM_EXPORT Module& getModule();
#endif
