
#include <db/Generated/Module.h>

#ifdef TOY_DB_REFLECT
#include <db/Generated/Convert.h>

#define TOY_DB_REFLECTION_IMPL
#include <db/Generated/Meta.h>
#endif

namespace toy
{
	toydb::toydb()
		: Module("toy::db")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        toyutil::module();

#ifdef TOY_DB_REFLECT
        // setup reflection meta data
		db_meta(*this);
#endif
	}
}

#ifdef TOY_DB_MODULE
extern "C"
TOY_DB_EXPORT Module& getModule()
{
	return toydb::module();
}
#endif
