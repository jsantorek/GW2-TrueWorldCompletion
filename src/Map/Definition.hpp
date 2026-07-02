#pragma once
#include "Map/Descriptor.hpp"

namespace TWC
{
struct MapDefinition : public MapDescriptor
{
    MapDefinition(const GW2RE::MapDef_t *def) : MapDescriptor(MapDescriptor::From(def)), Definition(def)
    {
    }
    inline operator uint32_t() const
    {
        return Definition->ID;
    }
    MapDefinition &operator=(MapDefinition &&other) noexcept
    {
        Definition = other.Definition;
        MapDescriptor::operator=(std::move(other));
        return *this;
    }
    MapDefinition &operator=(const MapDefinition &other)
    {
        Definition = other.Definition;
        MapDescriptor::operator=(std::move(other));
        return *this;
    }
    MapDefinition(const MapDefinition &def) : MapDescriptor(def), Definition(def.Definition)
    {
    }
    MapDefinition(MapDefinition &&def) noexcept : MapDescriptor(def), Definition(def.Definition)
    {
    }
    GW2RE::MapDef_t const *Definition;
};
} // namespace TWC
