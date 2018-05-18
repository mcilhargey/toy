
#include <editor/Generated/Module.h>

#ifdef TOY_EDITOR_REFLECT
#include <editor/Generated/Convert.h>

#define TOY_EDITOR_REFLECTION_IMPL
#include <editor/Generated/Meta.h>
#endif

namespace toy
{
	toyeditor::toyeditor()
		: Module("toy::editor")
	{
        // ensure dependencies are instantiated
        mudobj::module();
        mudmath::module();
        mudgeom::module();
        mudlang::module();
        toyutil::module();
        toycore::module();
        mudui::module();
        muduio::module();
        mudgfx::module();
        toyvisu::module();

#ifdef TOY_EDITOR_REFLECT
        // setup reflection meta data
		editor_meta(*this);
#endif
	}
}

#ifdef TOY_EDITOR_MODULE
extern "C"
TOY_EDITOR_EXPORT Module& getModule()
{
	return toyeditor::module();
}
#endif
