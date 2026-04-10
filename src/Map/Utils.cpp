#include "Utils.hpp"
#include "Configurable/ExpansionAssignment.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include <Content/Cache.hpp>
#include <Logging.hpp>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <optional>

std::optional<TWC::Expansion> TWC::MapUtils::MatchExpansions(ConfigurableExpansionAssignment assignment, uint32_t id1,
                                                             uint32_t id2)
{
    const auto dscr1 = G::Cache::Content->GetMapDescriptor(id1);
    if (!dscr1)
        return std::nullopt;
    const auto dscr2 = G::Cache::Content->GetMapDescriptor(id2);
    if (!dscr2)
        return std::nullopt;
    switch (assignment)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received, falling back to default {}",
            magic_enum::enum_type_name<ConfigurableExpansionAssignment>(), magic_enum::enum_name(assignment),
            magic_enum::enum_name(ConfigurableExpansionAssignment::AccessabilityBased));
    case ConfigurableExpansionAssignment::AccessabilityBased: {
        auto expansion = dscr1->GetExpansion<ConfigurableExpansionAssignment::AccessabilityBased>();
        if (expansion == dscr2->GetExpansion<ConfigurableExpansionAssignment::AccessabilityBased>())
            return expansion;
        break;
    }
    case ConfigurableExpansionAssignment::ChronologyBased:
        auto expansion = dscr1->GetExpansion<ConfigurableExpansionAssignment::ChronologyBased>();
        if (expansion == dscr2->GetExpansion<ConfigurableExpansionAssignment::ChronologyBased>())
            return expansion;
        break;
    }
    return std::nullopt;
}

std::optional<TWC::Continent> TWC::MapUtils::MatchContinent(uint32_t id1, uint32_t id2)
{
    const auto dscr1 = G::Cache::Content->GetMapDescriptor(id1);
    if (!dscr1)
        return std::nullopt;
    const auto dscr2 = G::Cache::Content->GetMapDescriptor(id2);
    if (!dscr2)
        return std::nullopt;
    auto continent = dscr1->GetContinent();
    if (continent == dscr2->GetContinent())
        return continent;
    return std::nullopt;
}