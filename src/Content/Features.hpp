#pragma once
#include <cstdint>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
enum class ContentFeatures : uint8_t
{
    StoryExclusive = 1 << 0,
    Retired = 1 << 1,
    UserExcluded = 1 << 2,
};
} // namespace TWC

template <> struct ::magic_enum::customize::enum_range<TWC::ContentFeatures>
{
    static constexpr bool is_flags = true;
};
