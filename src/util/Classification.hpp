#pragma once
#include "model/ContentDescriptor.hpp"

namespace TWC
{
inline auto DetermineExpansionChronologically(uint32_t mapId, uint32_t regionId)
{
    if (regionId == 10 || (mapId > 1165 && mapId < 1203))
        return ContentFeature::EXPANSION_HeartOfThorns;
    if (regionId == 12 || mapId == 1310)
        return ContentFeature::EXPANSION_PathOfFire;
    if (regionId == 37)
        return ContentFeature::EXPANSION_EndOfDragons;
    if (regionId == 48)
        return ContentFeature::EXPANSION_SecretsOfTheObscure;
    if (regionId == 18)
        return ContentFeature::EXPANSION_JanthirWilds;
    return ContentFeature::EXPANSION_None;
}

} // namespace TWC
