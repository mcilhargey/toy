
#include <block/Generated/Module.h>

#ifdef TOY_BLOCK_REFLECT
#include <block/Generated/Convert.h>

#define TOY_BLOCK_REFLECTION_IMPL
#include <block/Generated/Meta.h>
#endif

namespace toy
{
	toyblock::toyblock()
		: Module("toy::block")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        mudgeom::module();
        toyutil::module();
        toycore::module();
        mudui::module();
        muduio::module();
        mudgfx::module();
        toyvisu::module();
        mudedit::module();

#ifdef TOY_BLOCK_REFLECT
        // setup reflection meta data
		block_meta(*this);
#endif
	}
}

#ifdef TOY_BLOCK_MODULE
extern "C"
TOY_BLOCK_EXPORT Module& getModule()
{
	return toyblock::module();
}
#endif
