

#include <editor/Generated/Types.h>
#include <editor/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    
    template <> TOY_EDITOR_EXPORT Type& type<toy::ActionGroup>() { static Type ty("ActionGroup"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Clone>() { static Type ty("Clone"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Cut>() { static Type ty("Cut"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Edit>() { static Type ty("Edit"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Editor>() { static Type ty("Editor"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Paste>() { static Type ty("Paste"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Toolbelt>() { static Type ty("Toolbelt"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::Toolbox>() { static Type ty("Toolbox"); return ty; }
    template <> TOY_EDITOR_EXPORT Type& type<toy::DynamicToolbox>() { static Type ty("DynamicToolbox", type<toy::Toolbox>()); return ty; }
}
