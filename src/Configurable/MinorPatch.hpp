#pragma once
#include <magic_enum/magic_enum_containers.hpp>
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
enum class ConfigurableMinorPatch
{
    DiscoveryAlerts,
    LoadingScreens,
    SpiritValeMapLabelName,
    WorldMapProgress,
};
struct ConfigurableMinorPatches : magic_enum::containers::set<ConfigurableMinorPatch>
{
    using magic_enum::containers::set<ConfigurableMinorPatch>::set;
    auto static constexpr Default = {
        ConfigurableMinorPatch::DiscoveryAlerts,
        ConfigurableMinorPatch::LoadingScreens,
        ConfigurableMinorPatch::SpiritValeMapLabelName,
        ConfigurableMinorPatch::WorldMapProgress,
    };
    void Render();
    static magic_enum::containers::array<ConfigurableMinorPatch, std::string_view> MakePatchNames();
};
void to_json(nlohmann::json &, const ConfigurableMinorPatch &);
void from_json(const nlohmann::json &, ConfigurableMinorPatch &);
} // namespace TWC
