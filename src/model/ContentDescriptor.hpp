#pragma once
#include <cstdint>
#include <flags/flags.hpp>
#include <magic_enum/magic_enum.hpp>
#include <type_traits>
namespace TWC::Retired
{
enum class ContentFeature : uint16_t
{
    EXPANSION_None = 1 << 0,
    EXPANSION_HeartOfThorns = 1 << 1,
    EXPANSION_PathOfFire = 1 << 2,
    EXPANSION_EndOfDragons = 1 << 3,
    EXPANSION_SecretsOfTheObscure = 1 << 4,
    EXPANSION_JanthirWilds = 1 << 5,
    EXPANSION_VisionsOfEternity = 1 << 6,
    // When adding a new expansion, also update DescriptorMask::LatestExpansion
    EXPANSION_Optional = 1 << 7,
    CONTINENT_TheMists = 1 << 8,
    CONTINENT_Tyria = 1 << 9,
    MODE_PvE = 1 << 10,
    MODE_PvP = 1 << 11,
    MODE_WvW = 1 << 12,
    REWARD_GiftOfExplorationProgress = 1 << 13,
    REWARD_ZoneCompletionReward = 1 << 14
};
using ContentDescriptor = flags::flags<ContentFeature>;
} // namespace TWC::Retired

template <> struct ::flags::is_flags<TWC::Retired::ContentFeature> : std::true_type
{
};
template <> struct ::magic_enum::customize::enum_range<TWC::Retired::ContentFeature>
{
    static constexpr bool is_flags = true;
};
namespace TWC::DescriptorMask
{
constexpr auto LatestExpansion = Retired::ContentFeature::EXPANSION_VisionsOfEternity;
constexpr Retired::ContentDescriptor ThisOrEarlierExpansion(Retired::ContentDescriptor expansion)
{
    constexpr auto latest = static_cast<std::underlying_type_t<Retired::ContentFeature>>(LatestExpansion);
    if (expansion.underlying_value() > latest)
        expansion = LatestExpansion;
    for (auto bit = latest;
         bit >= static_cast<std::underlying_type_t<Retired::ContentFeature>>(Retired::ContentFeature::EXPANSION_None);
         bit >>= 1)
        if (bit <= expansion.underlying_value())
            expansion = expansion | static_cast<Retired::ContentFeature>(bit);
    return expansion;
}
consteval Retired::ContentDescriptor Expansions()
{
    Retired::ContentDescriptor dscr(flags::empty_t{});
    for (auto bit = static_cast<std::underlying_type_t<Retired::ContentFeature>>(LatestExpansion);
         bit > static_cast<std::underlying_type_t<Retired::ContentFeature>>(Retired::ContentFeature::EXPANSION_None);
         bit >>= 1)
        dscr = dscr | static_cast<Retired::ContentFeature>(bit);
    return dscr;
}
consteval Retired::ContentDescriptor Continents()
{
    return Retired::ContentFeature::CONTINENT_TheMists | Retired::ContentFeature::CONTINENT_Tyria;
}
consteval Retired::ContentDescriptor GameModes()
{
    return Retired::ContentFeature::MODE_PvE | Retired::ContentFeature::MODE_PvP | Retired::ContentFeature::MODE_WvW;
}
consteval Retired::ContentDescriptor Everything()
{
    return static_cast<Retired::ContentFeature>(
        (std::numeric_limits<std::underlying_type_t<Retired::ContentFeature>>::max)());
}
} // namespace TWC::DescriptorMask
