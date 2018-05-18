

#include <shell/Generated/Types.h>
#include <shell/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> TOY_SHELL_EXPORT Type& type<toy::GameShell>() { static Type ty("GameShell"); return ty; }
}
