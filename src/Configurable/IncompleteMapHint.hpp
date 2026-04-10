#pragma once
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
enum class ConfigurableIncompleteMapHint
{
    AllOpenedInApiLink,
    OneOpenedInWiki,
    OneCenteredOnWorldMap,

    Default = OneCenteredOnWorldMap
};
void to_json(nlohmann::json &, const ConfigurableIncompleteMapHint &);
void from_json(const nlohmann::json &, ConfigurableIncompleteMapHint &);
} // namespace TWC
