
#include <space/Generated/Module.h>

#ifdef _EX_SPACE_REFLECT
#include <space/Generated/Convert.h>

#define _EX_SPACE_REFLECTION_IMPL
#include <space/Generated/Meta.h>
#endif

	space::space()
		: Module("space")
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

#ifdef _EX_SPACE_REFLECT
        // setup reflection meta data
		space_meta(*this);
#endif
	}

#ifdef _SPACE_MODULE
extern "C"
_EX_SPACE_EXPORT Module& getModule()
{
	return space::module();
}
#endif
