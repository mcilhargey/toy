

#ifndef EX_15_SCRIPT_META_H
#define EX_15_SCRIPT_META_H

#include <15_script/Config.h>
#include <15_script/Forward.h>
#include <15_script/Bundle.h>
#include <15_script/Types.h>

#include <obj/Reflect/Meta.h>
#include <obj/Module/Module.h>

namespace mud
{
#ifdef EX_15_SCRIPT_REFLECTION_IMPL
    // Exported types
    
	template <> TOY_EX_15_SCRIPT_EXPORT Type& type<GameObject>() { static Type ty("GameObject", type<IdObject>()); return ty; }
	template <> TOY_EX_15_SCRIPT_EXPORT Type& type<MemberPointer<GameObject>>() { static Type ty("MemberPointer<GameObject>"); return ty; }
	template <> TOY_EX_15_SCRIPT_EXPORT Type& type<MethodPointer<GameObject>>() { static Type ty("MethodPointer<GameObject>"); return ty; }
    
    
#endif

    // Base Types
    
    // Enums
    
    // Sequences
    
    // Stores
    
    
    
    // GameObject
	template <>
	struct Reflection<GameObject> : public Meta
	{
        string m_name = "GameObject";
        size_t m_size = sizeof(GameObject);
        TypeClass m_type_class = OBJECT;
        std::vector<Type*> m_bases = { &type<IdObject>() };
        std::vector<uintptr_t> m_bases_offsets = { base_offset<GameObject, IdObject>() };
        bool m_serializable = false;
        struct {
            Member m_colour = { type<GameObject>(), member_var(&GameObject::m_colour), type<Colour>(), "colour", member_getter<Colour>(&GameObject::m_colour), member_setter<Colour>(&GameObject::m_colour), var(Colour()), MEMBER_VALUE|MEMBER_MUTABLE };
        } members;
        std::vector<Member> m_members = { members.m_colour };
        
        Reflection() : Meta(type<GameObject>()) { this->setup(*this); }
    };
        

    
    
        
    
#ifdef EX_15_SCRIPT_REFLECTION_IMPL
    void ex_15_script_meta(Module& module)
    {   
        rfl<GameObject>();
        module.m_types.push_back(&type<GameObject>());
    
    }
#endif

}

#endif // EX_15_SCRIPT_S_H
