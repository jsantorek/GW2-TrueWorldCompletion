#pragma once

#include "Content/Container.hpp"
#include "Map/Descriptor.hpp"

namespace TWC
{
template <template <ContentType> typename Collection>
struct MapEntity : public ContentContainer<Collection>, public MapDescriptor
{
    GW2RE::MapDef_t *Definition = nullptr;

    MapEntity(GW2RE::MapDef_t *def)
        : ContentContainer<Collection>(), MapDescriptor(MapDescriptor::From(def)), Definition(def)
    {
    }

    auto operator<=>(const MapEntity<Collection> &other) const
    {
        return this->Definition->ID <=> other.Definition->ID;
    };
};
} // namespace TWC
