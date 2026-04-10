#pragma once
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
enum class ConfigurableExpansionAssignment
{
    AccessabilityBased,
    ChronologyBased,

    Default = AccessabilityBased
};
void to_json(nlohmann::json &, const ConfigurableExpansionAssignment &);
void from_json(const nlohmann::json &, ConfigurableExpansionAssignment &);
} // namespace TWC
