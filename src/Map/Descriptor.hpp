#pragma once

#include "Game/Map/MapDef.h"
#include "Map/Features.hpp"
#include "Model/Expansion.hpp"
#include "Model/Festival.hpp"

namespace TWC
{
struct MapDescriptor
{
    flags::flags<MapFeatures> Features = flags::flags<TWC::MapFeatures>(flags::empty_t{});
    union {
        TWC::Expansion Expansion = TWC::Expansion::None;
        TWC::Festival Festival;
    };
    static MapDescriptor From(const GW2RE::MapDef_t *);
};
static_assert(static_cast<int>(Festival::None) == static_cast<int>(Expansion::None));
} // namespace TWC
