
#pragma once

#include <block/Generated/Types.h>

#include <obj/Reflect/Meta.h>
#include <obj/Reflect/Enum.h>
#include <obj/String/StringConvert.h>

namespace mud
{
    template <> inline void from_string(const string& str, toy::MatterState& val) { val = static_cast<toy::MatterState>(enu<toy::MatterState>().value(str.c_str())); };
    template <> inline void to_string(const toy::MatterState& val, string& str) { str = enu<toy::MatterState>().m_map[size_t(val)]; };
    
    
}
