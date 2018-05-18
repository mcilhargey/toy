
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
        
#include <shell/Generated/Forward.h>
#include <shell/Generated/Types.h>

#include <shell/Shell.h>

namespace toy
{
	class TOY_SHELL_EXPORT toyshell : public Module
	{
	private:
		toyshell();

	public:
		static toyshell& module() { static toyshell instance; return instance; }
	};
}

#ifdef TOY_SHELL_MODULE
extern "C"
TOY_SHELL_EXPORT Module& getModule();
#endif
