#pragma once

#include "Content/Features.hpp"
#include "Filter/Interface.hpp"
#include "Map/Features.hpp"

namespace TWC
{
struct FilterExcludedFeatures : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        return (dscr.Map.Features & ForbidenMapFeatures).any() || (dscr.Features & ForbidenContentFeatures).any();
    }
    magic_enum::containers::bitset<ContentFeatures> ForbidenContentFeatures{
        magic_enum::containers::detail::raw_access_t{}};
    magic_enum::containers::bitset<MapFeatures> ForbidenMapFeatures{magic_enum::containers::detail::raw_access_t{}};
};
} // namespace TWC
