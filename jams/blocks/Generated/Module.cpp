
#include <blocks/Generated/Module.h>

#ifdef _EX_BLOCKS_REFLECT
#include <blocks/Generated/Convert.h>

#define _EX_BLOCKS_REFLECTION_IMPL
#include <blocks/Generated/Meta.h>
#endif

	blocks::blocks()
		: Module("blocks")
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
        ex_05_character::module();

#ifdef _EX_BLOCKS_REFLECT
        // setup reflection meta data
		blocks_meta(*this);
#endif
	}

#ifdef _BLOCKS_MODULE
extern "C"
_EX_BLOCKS_EXPORT Module& getModule()
{
	return blocks::module();
}
#endif
