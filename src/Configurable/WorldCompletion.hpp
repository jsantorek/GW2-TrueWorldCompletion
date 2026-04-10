#pragma once
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
enum class ConfigurableWorldCompletion
{
    AllMapsCollectively,
    AllMapsWithCompletionReward,
    CurrentOrEarlierExpansionMaps,
    CurrentContinentMapsOnly,
    CurrentExpansionMapsOnly,

    Default = AllMapsCollectively,
};
void to_json(nlohmann::json &, const ConfigurableWorldCompletion &);
void from_json(const nlohmann::json &, ConfigurableWorldCompletion &);
} // namespace TWC
