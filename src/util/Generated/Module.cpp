
#include <util/Generated/Module.h>

#ifdef TOY_UTIL_REFLECT
#include <util/Generated/Convert.h>

#define TOY_UTIL_REFLECTION_IMPL
#include <util/Generated/Meta.h>
#endif

namespace toy
{
	toyutil::toyutil()
		: Module("toy::util")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();

#ifdef TOY_UTIL_REFLECT
        // setup reflection meta data
		util_meta(*this);
#endif
	}
}

#ifdef TOY_UTIL_MODULE
extern "C"
TOY_UTIL_EXPORT Module& getModule()
{
	return toyutil::module();
}
#endif
