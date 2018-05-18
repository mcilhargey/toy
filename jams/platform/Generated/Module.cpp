
#include <platform/Generated/Module.h>

#ifdef _EX_PLATFORM_REFLECT
#include <platform/Generated/Convert.h>

#define _EX_PLATFORM_REFLECTION_IMPL
#include <platform/Generated/Meta.h>
#endif

	platform::platform()
		: Module("platform")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        toyutil::module();
        toycore::module();
        toydb::module();
        mudui::module();
        muduio::module();
        mudgfx::module();
        toyvisu::module();
        mudedit::module();
        toyblock::module();
        mudgen::module();
        toyshell::module();

#ifdef _EX_PLATFORM_REFLECT
        // setup reflection meta data
		platform_meta(*this);
#endif
	}

#ifdef _PLATFORM_MODULE
extern "C"
_EX_PLATFORM_EXPORT Module& getModule()
{
	return platform::module();
}
#endif
