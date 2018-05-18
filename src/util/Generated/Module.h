
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
        
#include <util/Generated/Forward.h>
#include <util/Generated/Types.h>

#include <util/Procedure.h>
#include <util/Loader/DataLoader.h>
#include <util/Loader/Loader.h>
#include <util/Loader/MemberLoader.h>
#include <util/Threading/Scheduler.h>

namespace toy
{
	class TOY_UTIL_EXPORT toyutil : public Module
	{
	private:
		toyutil();

	public:
		static toyutil& module() { static toyutil instance; return instance; }
	};
}

#ifdef TOY_UTIL_MODULE
extern "C"
TOY_UTIL_EXPORT Module& getModule();
#endif
