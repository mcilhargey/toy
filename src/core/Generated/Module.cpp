
#include <core/Generated/Module.h>

#ifdef TOY_CORE_REFLECT
#include <core/Generated/Convert.h>

#define TOY_CORE_REFLECTION_IMPL
#include <core/Generated/Meta.h>
#endif

namespace toy
{
	toycore::toycore()
		: Module("toy::core")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        mudgeom::module();
        toyutil::module();

#ifdef TOY_CORE_REFLECT
        // setup reflection meta data
		core_meta(*this);
#endif
	}
}

#ifdef TOY_CORE_MODULE
extern "C"
TOY_CORE_EXPORT Module& getModule()
{
	return toycore::module();
}
#endif
