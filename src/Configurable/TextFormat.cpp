#include "Configurable/TextFormat.hpp"
#include "Format/CharacterProgressText.hpp"
#include "Format/CharacterProgressTooltip.hpp"
#include "Format/WorldMapProgress.hpp"
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

TWC::ConfigurableTextFormat TWC::ConfigurableTextFormat::Default()
{
    return ConfigurableTextFormat{
        .CharacterProgressText = FormatCharacterProgressText::Default,
        .CharacterProgressTooltip = FormatCharacterProgressTooltip::Default,
        .WorldMapProgress = FormatWorldMapProgress::Default,
    };
}

void TWC::ConfigurableTextFormat::Render()
{
    if (ImGui::Button("Use addon defaults"))
    {
        *this = Default();
        return;
    }

    ImGui::Text("Character selection screen - text");
    ImGui::PushID(magic_enum::enum_type_name<FormatCharacterProgressTextFlags>().data());
    for (const auto &[value, name] : magic_enum::enum_entries<FormatCharacterProgressTextFlags>())
    {
        ImGui::CheckboxFlags(name.data(), reinterpret_cast<int *>(&CharacterProgressText), static_cast<int>(value));
    }
    if (CharacterProgressText.none())
        ImGui::TextColored({1.f, 0, 0, 1.f}, "No text will be displayed");
    ImGui::PopID();
    ImGui::Separator();

    ImGui::PushID(magic_enum::enum_type_name<FormatCharacterProgressTooltipFlags>().data());
    ImGui::Text("Character selection screen - tooltip");
    for (const auto &[value, name] : magic_enum::enum_entries<FormatCharacterProgressTooltipFlags>())
    {
        ImGui::CheckboxFlags(name.data(), reinterpret_cast<int *>(&CharacterProgressTooltip), static_cast<int>(value));
    }
    if (CharacterProgressText.none())
        ImGui::Text("Non-modded tooltip will be used");
    ImGui::PopID();
    ImGui::Separator();

    ImGui::Text("World map completion progress bar");
    ImGui::PushID(magic_enum::enum_type_name<FormatWorldMapProgress>().data());
    for (const auto &[value, name] : magic_enum::enum_entries<FormatWorldMapProgress>())
    {
        if (ImGui::RadioButton(name.data(), WorldMapProgress == value))
        {
            WorldMapProgress = value;
        }
    }
    ImGui::PopID();
    ImGui::Separator();
}

void TWC::to_json(nlohmann::json &j, const ConfigurableTextFormat &c)
{
    j["WorldMapProgress"] = magic_enum::enum_name(c.WorldMapProgress);
    auto flags = nlohmann::json::array();
    for (const auto &[flag, name] : magic_enum::enum_entries<FormatCharacterProgressTextFlags>())
    {
        if (c.CharacterProgressText.test(flag))
            flags.emplace_back(name);
    }
    j["CharacterProgressText"] = flags;
    flags = nlohmann::json::array();
    for (const auto &[flag, name] : magic_enum::enum_entries<FormatCharacterProgressTooltipFlags>())
    {
        if (c.CharacterProgressTooltip.test(flag))
            flags.emplace_back(name);
    }
    j["CharacterProgressTooltip"] = flags;
}

void TWC::from_json(const nlohmann::json &j, ConfigurableTextFormat &c)
{
    auto name = j.at("WorldMapProgress").get<std::string>();
    if (auto casted = magic_enum::enum_cast<FormatWorldMapProgress>(name))
    {
        c.WorldMapProgress = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
    c.CharacterProgressText.reset();
    for (const auto &flag : j.at("CharacterProgressText").get<std::vector<std::string>>())
    {

        if (auto casted = magic_enum::enum_cast<FormatCharacterProgressTextFlags>(flag))
        {
            c.CharacterProgressText.set(casted.value());
        }
        else
        {
            throw std::runtime_error(flag);
        }
    }
    c.CharacterProgressTooltip.reset();
    for (const auto &flag : j.at("CharacterProgressTooltip").get<std::vector<std::string>>())
    {

        if (auto casted = magic_enum::enum_cast<FormatCharacterProgressTooltipFlags>(flag))
        {
            c.CharacterProgressTooltip.set(casted.value());
        }
        else
        {
            throw std::runtime_error(flag);
        }
    }
}
