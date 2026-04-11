#include "imgui.h"
#include <Configurable/MinorPatch.hpp>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <string_view>

void TWC::ConfigurableMinorPatches::Render()
{
    static auto names = MakePatchNames();
    for (const auto value : magic_enum::enum_values<ConfigurableMinorPatch>())
    {
        auto isEnabled = contains(value);
        if (ImGui::Checkbox(names[value].data(), &isEnabled))
        {
            if (isEnabled)
                insert(value);
            else
                erase(value);
        }
    }
}
magic_enum::containers::array<TWC::ConfigurableMinorPatch, std::string_view> TWC::ConfigurableMinorPatches::
    MakePatchNames()
{
    auto names = magic_enum::containers::array<ConfigurableMinorPatch, std::string_view>{};
    names.fill(""); /* TODO */
    names[TWC::ConfigurableMinorPatch::DiscoveryAlerts] =
        "Loading screen of all maps with discoverable content present displays completion progress values";
    names[ConfigurableMinorPatch::LoadingScreens] =
        "Content discovered alerts on all maps display completion progress bar";
    names[ConfigurableMinorPatch::SpiritValeMapLabelName] =
        "Progress widget displayed after hovering over Spirit Vale's map label uses correct name";
    names[ConfigurableMinorPatch::WorldMapProgress] =
        "World completion widget displayed after opening World Map on any map";
    return names;
}