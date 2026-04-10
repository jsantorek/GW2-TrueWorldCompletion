#pragma once
#include <cstdint>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
enum class MapFeatures : uint8_t
{
    Accessible_CoreAccount = 1 << 0, // 01
    Accessible_Festival = 1 << 1,    // 02
    Accessible_Gemstore = 1 << 2,    // 04
    Competitive_PvP = 1 << 3,        // 08
    Competitive_WvW = 1 << 4,        // 10
    Continent_Mists = 1 << 5,        // 20
    Continent_Tyria = 1 << 6,        // 40
    HasCompletionReward = 1 << 7,    // 80
};
} // namespace TWC

template <> struct ::magic_enum::customize::enum_range<TWC::MapFeatures>
{
    static constexpr bool is_flags = true;
};
