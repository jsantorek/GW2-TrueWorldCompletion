#pragma once
#include <cstdint>
#include <flags/flags.hpp>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
enum class MapFeatures : uint8_t
{
    Accessible_FreeAccount = 1 << 0, // 01
    Accessible_Festival = 1 << 1,    // 02
    Competitive_PvP = 1 << 2,        // 04
    Competitive_WvW = 1 << 3,        // 08
    Continent_Mists = 1 << 4,        // 10
    Continent_Tyria = 1 << 5,        // 20
    HasCompletionReward = 1 << 6,    // 40
};
} // namespace TWC

template <> struct ::flags::is_flags<TWC::MapFeatures> : std::true_type
{
};
template <> struct ::magic_enum::customize::enum_range<TWC::MapFeatures>
{
    static constexpr bool is_flags = true;
};
