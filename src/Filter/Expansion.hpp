#pragma once

#include "Filter/Interface.hpp"
#include "Map/Features.hpp"
#include "Model/Expansion.hpp"

namespace TWC
{
template <Expansion E> struct FilterExpansion : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        if (dscr.Features.count(ContentFeatures::UserExcluded))
            return false;
        if constexpr (E == Expansion::None)
        {
            return dscr.Map.Features.count(MapFeatures::Accessible_Festival) || dscr.Map.Expansion == Expansion::None;
        }
        else
        {
            return dscr.Map.Features.count(MapFeatures::Accessible_Festival) == 0 && dscr.Map.Expansion == E;
        }
    }
};

} // namespace TWC