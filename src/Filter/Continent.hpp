#pragma once

#include "Filter/Interface.hpp"
#include "Map/Descriptor.hpp"
#include "Map/Features.hpp"
#include "Model/Continent.hpp"

namespace TWC
{
struct FilterContinent : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        if (Continent == Continent::Tyria)
        {
            return dscr.Map.Features.test(MapFeatures::Continent_Tyria);
        }
        else if (Continent == Continent::Mists)
        {
            return dscr.Map.Features.test(MapFeatures::Continent_Mists);
        }
        return false;
    }

    inline FilterContinent(MapDescriptor map) : Continent(map.GetContinent())
    {
    }

    Continent Continent;
};

} // namespace TWC