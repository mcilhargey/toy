

#pragma once

#include <block/Generated/Module.h>

#include <obj/Any.h>
#include <obj/Reflect/MetaDecl.h>
#include <obj/System/System.h>

namespace mud
{
    
#ifdef TOY_BLOCK_REFLECTION_IMPL
    void block_meta(Module& module)
    {   
    // Base Types
    
    // Enums
    {
        static Meta meta = { type<toy::MatterState>(), &namspc({ "toy" }), "MatterState", sizeof(toy::MatterState), TypeClass::Enum };
        static Enum enu = { type<toy::MatterState>(),
            false,
            { "STATE_SOLID", "STATE_LIQUID", "STATE_GAS", "STATE_PLASMA" },
            { 0, 1, 2, 3 },
            { var(toy::STATE_SOLID), var(toy::STATE_LIQUID), var(toy::STATE_GAS), var(toy::STATE_PLASMA) }
        };
        meta_enum<toy::MatterState>();
    }
    
    
    // Sequences
    {
        static Meta meta = { type<std::vector<toy::Block*>>(), &namspc({}), "std::vector<toy::Block*>", sizeof(std::vector<toy::Block*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<toy::Block*>>() };
        cls.m_content = &type<toy::Block>();
        meta_sequence<std::vector<toy::Block*>, toy::Block*>();
    }
    
    {
        static Meta meta = { type<std::vector<toy::Sector*>>(), &namspc({}), "std::vector<toy::Sector*>", sizeof(std::vector<toy::Sector*>), TypeClass::Sequence };
        static Class cls = { type<std::vector<toy::Sector*>>() };
        cls.m_content = &type<toy::Sector>();
        meta_sequence<std::vector<toy::Sector*>, toy::Sector*>();
    }
    
    
    
    
        
    // toy::Block
    {
        static Meta meta = { type<toy::Block>(), &namspc({ "toy" }), "Block", sizeof(toy::Block), TypeClass::Object };
        static Class cls = { type<toy::Block>(),
            // bases
            {  },
            {  },
            // constructors
            {
                { type<toy::Block>(), [](Ref ref, array<Var> args) { new(&val<toy::Block>(ref)) toy::Block( val<toy::Entity>(args[0]), val<toy::Emitter>(args[1]), &val<toy::Block>(args[2]), val<size_t>(args[3]), val<float>(args[4]) ); }, { { "entity", Ref(type<toy::Entity>()) }, { "emitter", Ref(type<toy::Emitter>()) }, { "parentblock", Ref(type<toy::Block>()), Param::Nullable }, { "index", var(size_t()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::Block>(), Address(), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<toy::Block>(), Address(), type<toy::Emitter>(), "emitter", Ref(type<toy::Emitter>()), Member::None },
                { type<toy::Block>(), member_address(&toy::Block::m_parentblock), type<toy::Block>(), "parentblock", Ref(type<toy::Block>()), Member::Flags(Member::Pointer|Member::Link) },
                { type<toy::Block>(), member_address(&toy::Block::m_index), type<size_t>(), "index", var(size_t()), Member::Value },
                { type<toy::Block>(), member_address(&toy::Block::m_size), type<float>(), "size", var(float()), Member::Value },
                { type<toy::Block>(), member_address(&toy::Block::m_updated), type<size_t>(), "updated", var(size_t()), Member::Value }
            },
            // methods
            {
                { type<toy::Block>(), "subdivide", member_address(&toy::Block::subdivide), [](Ref object, array<Var> args, Var& result) { UNUSED(result); UNUSED(args);val<toy::Block>(object).subdivide(); }, {}, Var() },
                { type<toy::Block>(), "reset", member_address(&toy::Block::reset), [](Ref object, array<Var> args, Var& result) { UNUSED(result); UNUSED(args);val<toy::Block>(object).reset(); }, {}, Var() },
                { type<toy::Block>(), "chunk", member_address(&toy::Block::chunk), [](Ref object, array<Var> args, Var& result) { UNUSED(result); val<toy::Block>(object).chunk(val<size_t>(args[0]), val<size_t>(args[1]), val<size_t>(args[2]), val<toy::Element>(args[3])); }, { { "x", var(size_t()) }, { "y", var(size_t()) }, { "z", var(size_t()) }, { "element", Ref(type<toy::Element>()) } }, Var() },
                { type<toy::Block>(), "commit", member_address(&toy::Block::commit), [](Ref object, array<Var> args, Var& result) { UNUSED(result); UNUSED(args);val<toy::Block>(object).commit(); }, {}, Var() }
            },
            // static members
            {
            }
        };
        
        
        init_pool<toy::Block>(); 
        
        meta_class<toy::Block>();
    }
    
    
    
    
        
    // toy::Element
    {
        static Meta meta = { type<toy::Element>(), &namspc({ "toy" }), "Element", sizeof(toy::Element), TypeClass::Object };
        static Class cls = { type<toy::Element>(),
            // bases
            {  },
            {  },
            // constructors
            {
                { type<toy::Element>(), [](Ref ref, array<Var> args) { new(&val<toy::Element>(ref)) toy::Element( val<cstring>(args[0]), val<toy::MatterState>(args[1]), val<mud::Colour>(args[2]) ); }, { { "name", var(cstring()) }, { "state", var(toy::MatterState()) }, { "colour", var(mud::Colour()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::Element>(), member_address(&toy::Element::m_id), type<mud::Id>(), "id", var(mud::Id()), Member::Value },
                { type<toy::Element>(), member_address(&toy::Element::m_name), type<std::string>(), "name", var(std::string()), Member::Value },
                { type<toy::Element>(), member_address(&toy::Element::m_matterState), type<toy::MatterState>(), "matterState", var(toy::MatterState()), Member::Value },
                { type<toy::Element>(), member_address(&toy::Element::m_colour), type<mud::Colour>(), "colour", var(mud::Colour()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        init_pool<toy::Element>(); 
        
        meta_class<toy::Element>();
    }
    
    
        
    // mud::Grid<toy::Block*>
    {
        static Meta meta = { type<mud::Grid<toy::Block*>>(), &namspc({ "mud" }), "Grid<toy::Block*>", sizeof(mud::Grid<toy::Block*>), TypeClass::Struct };
        static Class cls = { type<mud::Grid<toy::Block*>>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<mud::Grid<toy::Block*>>(), [](Ref ref, Ref other) { new(&val<mud::Grid<toy::Block*>>(ref)) mud::Grid<toy::Block*>(val<mud::Grid<toy::Block*>>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::Grid<toy::Block*>>();
    }
    
    
    
    
    
    
    
    
    
    
    
    
        
    // toy::OBlock
    {
        static Meta meta = { type<toy::OBlock>(), &namspc({ "toy" }), "OBlock", sizeof(toy::OBlock), TypeClass::Object };
        static Class cls = { type<toy::OBlock>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<toy::OBlock, mud::Construct>() },
            // constructors
            {
                { type<toy::OBlock>(), [](Ref ref, array<Var> args) { new(&val<toy::OBlock>(ref)) toy::OBlock( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), &val<toy::Block>(args[3]), val<size_t>(args[4]), val<float>(args[5]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "parentblock", Ref(type<toy::Block>()), Param::Nullable }, { "index", var(size_t()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::OBlock>(), member_address(&toy::OBlock::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<toy::OBlock>(); 
        
        meta_class<toy::OBlock>();
    }
    
    
        
    // toy::Chunk
    {
        static Meta meta = { type<toy::Chunk>(), &namspc({ "toy" }), "Chunk", sizeof(toy::Chunk), TypeClass::Object };
        static Class cls = { type<toy::Chunk>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<toy::Chunk, mud::Construct>() },
            // constructors
            {
                { type<toy::Chunk>(), [](Ref ref, array<Var> args) { new(&val<toy::Chunk>(ref)) toy::Chunk( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<size_t>(args[3]), val<toy::Element>(args[4]), val<float>(args[5]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "index", var(size_t()) }, { "element", Ref(type<toy::Element>()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::Chunk>(), member_address(&toy::Chunk::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<toy::Chunk>(), member_address(&toy::Chunk::m_index), type<size_t>(), "index", var(size_t()), Member::Value },
                { type<toy::Chunk>(), Address(), type<toy::Block>(), "block", Ref(type<toy::Block>()), Member::None },
                { type<toy::Chunk>(), Address(), type<toy::Element>(), "element", Ref(type<toy::Element>()), Member::None },
                { type<toy::Chunk>(), member_address(&toy::Chunk::m_size), type<float>(), "size", var(float()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<toy::Chunk>(); 
        
        meta_class<toy::Chunk>();
    }
    
    
        
    // toy::Heap
    {
        static Meta meta = { type<toy::Heap>(), &namspc({ "toy" }), "Heap", sizeof(toy::Heap), TypeClass::Object };
        static Class cls = { type<toy::Heap>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<toy::Heap, mud::Construct>() },
            // constructors
            {
                { type<toy::Heap>(), [](Ref ref, array<Var> args) { new(&val<toy::Heap>(ref)) toy::Heap( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<toy::Element>(args[3]), val<float>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "element", Ref(type<toy::Element>()) }, { "radius", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::Heap>(), member_address(&toy::Heap::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<toy::Heap>(), Address(), type<toy::Element>(), "element", Ref(type<toy::Element>()), Member::Link },
                { type<toy::Heap>(), member_address(&toy::Heap::m_radius), type<float>(), "radius", var(float()), Member::Value }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<toy::Heap>(); 
        
        meta_class<toy::Heap>();
    }
    
    
        
    // toy::Sector
    {
        static Meta meta = { type<toy::Sector>(), &namspc({ "toy" }), "Sector", sizeof(toy::Sector), TypeClass::Object };
        static Class cls = { type<toy::Sector>(),
            // bases
            { &type<mud::Construct>() },
            { base_offset<toy::Sector, mud::Construct>() },
            // constructors
            {
                { type<toy::Sector>(), [](Ref ref, array<Var> args) { new(&val<toy::Sector>(ref)) toy::Sector( val<mud::Id>(args[0]), val<toy::Entity>(args[1]), val<mud::vec3>(args[2]), val<mud::uvec3>(args[3]), val<float>(args[4]) ); }, { { "id", var(mud::Id()) }, { "parent", Ref(type<toy::Entity>()) }, { "position", var(mud::vec3()) }, { "coordinate", var(mud::uvec3()) }, { "size", var(float()) } } }
            },
            // copy constructor
            {
            },
            // members
            {
                { type<toy::Sector>(), member_address(&toy::Sector::m_entity), type<toy::Entity>(), "entity", Ref(type<toy::Entity>()), Member::None },
                { type<toy::Sector>(), member_address(&toy::Sector::m_coordinate), type<mud::uvec3>(), "coordinate", var(mud::uvec3()), Member::Value },
                { type<toy::Sector>(), member_address(&toy::Sector::m_size), type<float>(), "size", var(float()), Member::Value },
                { type<toy::Sector>(), member_address(&toy::Sector::m_block), type<toy::Block>(), "block", Ref(type<toy::Block>()), Member::Flags(Member::Pointer|Member::Mutable|Member::Link) }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        cls.m_root = &type<toy::Entity>();
        
        init_pool<toy::Sector>(); 
        
        meta_class<toy::Sector>();
    }
    

    
        module.m_types.push_back(&type<toy::Block>());
        module.m_types.push_back(&type<toy::Element>());
        module.m_types.push_back(&type<mud::Grid<toy::Block*>>());
        module.m_types.push_back(&type<toy::MatterState>());
        module.m_types.push_back(&type<std::vector<toy::Block*>>());
        module.m_types.push_back(&type<std::vector<toy::Sector*>>());
        module.m_types.push_back(&type<toy::OBlock>());
        module.m_types.push_back(&type<toy::Chunk>());
        module.m_types.push_back(&type<toy::Heap>());
        module.m_types.push_back(&type<toy::Sector>());
    
        {
            auto func = [](array<Var> args, Var& result) { UNUSED(result);  toy::index_blocks(val<mud::ivec3>(args[0]), val<mud::Grid<toy::Block*>>(args[1]), val<std::vector<toy::Block*>>(args[2]), val<std::vector<toy::Sector*>>(args[3])); };
            std::vector<Param> params = { { "grid_size", var(mud::ivec3()) }, { "grid", var(mud::Grid<toy::Block*>()) }, { "blocks", var(std::vector<toy::Block*>()) }, { "sectors", var(std::vector<toy::Sector*>()) } };
            module.m_functions.push_back({ &namspc({ "toy" }), "index_blocks", function_id<void(*)(const mud::ivec3&, mud::Grid<toy::Block*>&, const std::vector<toy::Block*>&, const std::vector<toy::Sector*>&)>(&toy::index_blocks), func, params, Var() });
        }
    }
#endif

}
