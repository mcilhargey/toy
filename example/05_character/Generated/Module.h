
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
        
#include <05_character/Generated/Forward.h>
#include <05_character/Generated/Types.h>

#include <05_character/05_character.h>

	class _05_CHARACTER_EXPORT ex_05_character : public Module
	{
	private:
		ex_05_character();

	public:
		static ex_05_character& module() { static ex_05_character instance; return instance; }
	};

#ifdef _EX_05_CHARACTER_MODULE
extern "C"
_05_CHARACTER_EXPORT Module& getModule();
#endif
