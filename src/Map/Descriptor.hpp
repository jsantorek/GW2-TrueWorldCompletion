#pragma once

#include "Configurable/ExpansionAssignment.hpp"
#include "Game/Map/MapDef.h"
#include "Map/Features.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include "Model/Festival.hpp"
#include <magic_enum/magic_enum_containers.hpp>
namespace TWC
{
struct MapDescriptor
{
    magic_enum::containers::bitset<MapFeatures> Features{magic_enum::containers::detail::raw_access_t{}};

    template <ConfigurableExpansionAssignment Assignment> inline Expansion GetExpansion() const
    {
        if (Features.test(MapFeatures::Accessible_Festival))
            return Expansion::None;
        if constexpr (Assignment == ConfigurableExpansionAssignment::AccessabilityBased)
        {
            if (Features.test(MapFeatures::Accessible_CoreAccount))
                return Expansion::None;
            return Expansion;
        }
        else if constexpr (Assignment == ConfigurableExpansionAssignment::ChronologyBased)
        {
            return Expansion;
        }
        else
        {
            static_assert(Assignment != Assignment, "Unsupported expansion assignment method!");
        }
    }

    inline Festival GetFestival() const
    {
        if (Features.test(MapFeatures::Accessible_Festival))
            return Festival;
        return Festival::None;
    }

    inline Continent GetContinent() const
    {
        if (Features.test(MapFeatures::Continent_Tyria))
            return Continent::Tyria;
        if (Features.test(MapFeatures::Continent_Mists))
            return Continent::Mists;
        return Continent::None;
    }

    static MapDescriptor From(const GW2RE::MapDef_t *);
    static std::optional<MapDescriptor> FromCurrentMap();

  private:
    union {
        TWC::Expansion Expansion = TWC::Expansion::None;
        TWC::Festival Festival;
    };
};
static_assert(static_cast<int>(Festival::None) == static_cast<int>(Expansion::None));
} // namespace TWC
