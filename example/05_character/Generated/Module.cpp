
#include <05_character/Generated/Module.h>

#ifdef _05_CHARACTER_REFLECT
#include <05_character/Generated/Convert.h>

#define _05_CHARACTER_REFLECTION_IMPL
#include <05_character/Generated/Meta.h>
#endif

	ex_05_character::ex_05_character()
		: Module("ex_05_character")
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

#ifdef _05_CHARACTER_REFLECT
        // setup reflection meta data
		ex_05_character_meta(*this);
#endif
	}

#ifdef _EX_05_CHARACTER_MODULE
extern "C"
_05_CHARACTER_EXPORT Module& getModule()
{
	return ex_05_character::module();
}
#endif
