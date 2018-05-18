
#include <shell/Generated/Module.h>

#ifdef TOY_SHELL_REFLECT
#include <shell/Generated/Convert.h>

#define TOY_SHELL_REFLECTION_IMPL
#include <shell/Generated/Meta.h>
#endif

namespace toy
{
	toyshell::toyshell()
		: Module("toy::shell")
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

#ifdef TOY_SHELL_REFLECT
        // setup reflection meta data
		shell_meta(*this);
#endif
	}
}

#ifdef TOY_SHELL_MODULE
extern "C"
TOY_SHELL_EXPORT Module& getModule()
{
	return toyshell::module();
}
#endif
