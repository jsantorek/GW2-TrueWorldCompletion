#pragma once

#include "model/MapClassification.hpp"
#include <Windows.h>

namespace TWC::Utils
{
inline MapClassification::MapExpansion DetermineExpansionChronologically(uint32_t mapId, uint32_t regionId)
{
    if (regionId == 10 || (mapId > 1165 && mapId < 1203))
        return MapClassification::HeartOfThorns;
    if (regionId == 12 || mapId == 1310)
        return MapClassification::PathOfFire;
    if (regionId == 37)
        return MapClassification::EndOfDragons;
    if (regionId == 48)
        return MapClassification::SecretsOfTheObscure;
    if (regionId == 18)
        return MapClassification::JanthirWilds;
    return MapClassification::Core;
}

} // namespace TWC::Utils
