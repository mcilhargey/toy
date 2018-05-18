

#include <block/Generated/Types.h>
#include <block/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    template <> TOY_BLOCK_EXPORT Type& type<toy::MatterState>() { static Type ty("toy::MatterState"); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<std::vector<toy::Block*>>() { static Type ty("std::vector<toy::Block*>"); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<std::vector<toy::Sector*>>() { static Type ty("std::vector<toy::Sector*>"); return ty; }
    
    template <> TOY_BLOCK_EXPORT Type& type<toy::Block>() { static Type ty("Block"); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<toy::Element>() { static Type ty("Element"); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<mud::Grid<toy::Block*>>() { static Type ty("Grid<toy::Block*>"); return ty; }
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::OBlock>() { static Prototype ty("toy::OBlock", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::Block>() }); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<toy::OBlock>() { return proto<toy::OBlock>(); }
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Chunk>() { static Prototype ty("toy::Chunk", type<toy::Entity>(), {  }); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<toy::Chunk>() { return proto<toy::Chunk>(); }
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Heap>() { static Prototype ty("toy::Heap", type<toy::Entity>(), {  }); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<toy::Heap>() { return proto<toy::Heap>(); }
    template <> TOY_BLOCK_EXPORT Prototype& proto<toy::Sector>() { static Prototype ty("toy::Sector", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::WorldPage>(), &type<toy::BufferPage>(), &type<toy::Navblock>() }); return ty; }
    template <> TOY_BLOCK_EXPORT Type& type<toy::Sector>() { return proto<toy::Sector>(); }
}
