#pragma once
#include "Map/Descriptor.hpp"

namespace TWC
{
struct MapDefinition : public MapDescriptor
{
    MapDefinition(GW2RE::MapDef_t *def) : MapDescriptor(MapDescriptor::From(def)), Definition(def)
    {
    }
    inline operator uint32_t() const
    {
        return Definition->ID;
    }
    GW2RE::MapDef_t *Definition = nullptr;
};
} // namespace TWC
