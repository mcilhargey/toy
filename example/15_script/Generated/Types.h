#ifndef EX_15_SCRIPT_TYPES_GEN_H
#define EX_15_SCRIPT_TYPES_GEN_H

#include <15_script/Config.h>
#include <15_script/Forward.h>

#include <obj/Type.h>

namespace mud
{   
    // Exported types
    
    template <> TOY_EX_15_SCRIPT_EXPORT Type& type<GameObject>();
	template <> TOY_EX_15_SCRIPT_EXPORT Type& type<MemberPointer<GameObject>>();
	template <> TOY_EX_15_SCRIPT_EXPORT Type& type<MethodPointer<GameObject>>();
}

#endif // EX_15_SCRIPT_TYPES_GEN_H
