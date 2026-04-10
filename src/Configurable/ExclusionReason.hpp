#pragma once
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
enum class ConfigurableExclusionReason
{
    Historical,
    Exclusive_Gemstore,
    Exclusive_Story,
    Exclusive_Festival,
    Exclusive_PvP,
    Exclusive_WvW,
};
void to_json(nlohmann::json &, const ConfigurableExclusionReason &);
void from_json(const nlohmann::json &, ConfigurableExclusionReason &);
} // namespace TWC
