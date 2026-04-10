#pragma once

#include "Configurable/ExpansionAssignment.hpp"
#include "Filter/Interface.hpp"
#include "Map/Descriptor.hpp"
#include "Model/Expansion.hpp"

namespace TWC
{
template <ConfigurableExpansionAssignment Assignment> struct FilterExpansion : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        return dscr.Map.GetExpansion<Assignment>() == Expansion;
    }
    inline FilterExpansion(MapDescriptor map) : Expansion(map.GetExpansion<Assignment>())
    {
    }
    inline FilterExpansion(Expansion exp) : Expansion(exp)
    {
    }

    Expansion Expansion;
};
template <ConfigurableExpansionAssignment Assignment>
struct FilterSubsequentExpansion : public FilterExpansion<Assignment>
{
    using FilterExpansion<Assignment>::FilterExpansion;
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        return dscr.Map.GetExpansion<Assignment>() <= FilterExpansion<Assignment>::Expansion;
    }
};
} // namespace TWC