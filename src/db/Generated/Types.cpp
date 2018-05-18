

#include <db/Generated/Types.h>
#include <db/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> TOY_DB_EXPORT Type& type<toy::SqliteDatabase>() { static Type ty("SqliteDatabase"); return ty; }
}
