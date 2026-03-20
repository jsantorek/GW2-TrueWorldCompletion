#pragma once

#include "Constant/ContinentIdentifier.hpp"
#include "Filter/Interface.hpp"
#include "Map/Features.hpp"

namespace TWC
{

template <int ContinentIdentifier> struct FilterContinent : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        if (dscr.Features.count(ContentFeatures::UserExcluded))
            return false;
        if constexpr (ContinentIdentifier == ConstantContinentIdentifier::Tyria)
        {
            return dscr.Map.Features.count(MapFeatures::Continent_Tyria);
        }
        else if constexpr (ContinentIdentifier == ConstantContinentIdentifier::Mists)
        {
            return dscr.Map.Features.count(MapFeatures::Continent_Mists);
        }
        else
        {
            static_assert(ContinentIdentifier != ContinentIdentifier,
                          "Couldn't identify flag for provided Continent Identifier!");
        }
    }
};

} // namespace TWC