
#include <visu/Generated/Module.h>

#ifdef TOY_VISU_REFLECT
#include <visu/Generated/Convert.h>

#define TOY_VISU_REFLECTION_IMPL
#include <visu/Generated/Meta.h>
#endif

namespace toy
{
	toyvisu::toyvisu()
		: Module("toy::visu")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        mudgeom::module();
        toyutil::module();
        toycore::module();
        mudui::module();
        mudgfx::module();
        mudsnd::module();

#ifdef TOY_VISU_REFLECT
        // setup reflection meta data
		visu_meta(*this);
#endif
	}
}

#ifdef TOY_VISU_MODULE
extern "C"
TOY_VISU_EXPORT Module& getModule()
{
	return toyvisu::module();
}
#endif
