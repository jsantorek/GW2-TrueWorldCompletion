#pragma once
#include <cstdint>
#include <flags/flags.hpp>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
enum class ContentFeatures : uint8_t
{
    StoryExclusive = 1 << 0,
    GemstoreExclusive = 1 << 1,
    Retired = 1 << 2,
    UserExcluded = 1 << 3,
};
} // namespace TWC

template <> struct ::flags::is_flags<TWC::ContentFeatures> : std::true_type
{
};
template <> struct ::magic_enum::customize::enum_range<TWC::ContentFeatures>
{
    static constexpr bool is_flags = true;
};
