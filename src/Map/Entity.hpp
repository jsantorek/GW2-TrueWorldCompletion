#pragma once

#include "Content/Container.hpp"
#include "Map/Definition.hpp"

namespace TWC
{
template <template <ContentType> typename Collection>
struct MapEntity : public ContentContainer<Collection>, public MapDefinition
{
    MapEntity(GW2RE::MapDef_t *def) : ContentContainer<Collection>(), MapDefinition(def)
    {
    }

    MapEntity(const MapDefinition &other) : ContentContainer<Collection>(), MapDefinition(other)
    {
    }

    auto operator<=>(const MapEntity<Collection> &other) const
    {
        return this->Definition->ID <=> other.Definition->ID;
    };
};
} // namespace TWC
