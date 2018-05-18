
#pragma once

#include <space/Generated/Types.h>

#include <obj/Reflect/Meta.h>
#include <obj/Reflect/Enum.h>
#include <obj/String/StringConvert.h>

namespace mud
{
    template <> inline void from_string(const string& str, FleetStance& val) { val = static_cast<FleetStance>(enu<FleetStance>().value(str.c_str())); };
    template <> inline void to_string(const FleetStance& val, string& str) { str = enu<FleetStance>().m_map[size_t(val)]; };
    
    template <> inline void from_string(const string& str, WeaponType& val) { val = static_cast<WeaponType>(enu<WeaponType>().value(str.c_str())); };
    template <> inline void to_string(const WeaponType& val, string& str) { str = enu<WeaponType>().m_map[size_t(val)]; };
    
    
}
