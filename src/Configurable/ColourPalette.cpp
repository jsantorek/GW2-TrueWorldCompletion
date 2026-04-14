#include "ColourPalette.hpp"
#include "Configurable/Colour.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <imgui.h>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string_view>

namespace
{
consteval auto rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return std::array<float, 4>{static_cast<float>(r) / static_cast<float>(std::numeric_limits<uint8_t>::max()),
                                static_cast<float>(g) / static_cast<float>(std::numeric_limits<uint8_t>::max()),
                                static_cast<float>(b) / static_cast<float>(std::numeric_limits<uint8_t>::max()), 1.f};
}
constexpr auto CeilByte = 255.5f;
constexpr auto convert(std::array<float, 4> rgba)
{
    auto col = GW2RE::Colour4{};
    col.R = static_cast<uint8_t>(std::clamp(rgba[0], 0.0f, 1.0f) * CeilByte);
    col.G = static_cast<uint8_t>(std::clamp(rgba[1], 0.0f, 1.0f) * CeilByte);
    col.B = static_cast<uint8_t>(std::clamp(rgba[2], 0.0f, 1.0f) * CeilByte);
    col.A = static_cast<uint8_t>(std::clamp(rgba[3], 0.0f, 1.0f) * CeilByte);
    return col;
}
constexpr auto GameDefaultColour = rgb(49, 139, 188);
} // namespace

void TWC::ConfigurableColourPalette::Render()
{
    static const auto Names = MakeColourNames();
    if (ImGui::Button("Use addon defaults"))
    {
        *this = Default();
    }
    ImGui::SameLine();
    if (ImGui::Button("Use game defaults"))
    {
        for (auto &colour : *this)
        {
            colour = GameDefaultColour;
        }
    }
    for (auto const &value : magic_enum::enum_values<TWC::ConfigurableColour>())
    {
        ImGui::ColorEdit4(Names.at(value).data(), at(value).data());
    }
}

magic_enum::containers::array<TWC::ConfigurableColour, std::string_view> TWC::ConfigurableColourPalette::
    MakeColourNames()
{
    auto names = magic_enum::containers::array<ConfigurableColour, std::string_view>{};
    names.fill(""); /* TODO */
    names[ConfigurableColour::Legendary] = "Legendary";
    names[ConfigurableColour::Reward] = "Reward";
    names[ConfigurableColour::Continent_Mists] = "Continent: Mists";
    names[ConfigurableColour::Continent_Tyria] = "Continent: Tyria";
    names[ConfigurableColour::Core] = "Core Game";
    names[ConfigurableColour::Expansion_HeartOfThorns] = "Expansion: Heart of Thorns";
    names[ConfigurableColour::Expansion_PathOfFire] = "Expansion: Path of Fire";
    names[ConfigurableColour::Expansion_EndOfDragons] = "Expansion: End of Dragons";
    names[ConfigurableColour::Expansion_SecretsOfTheObscure] = "Expansion: Secrets of the Obscure";
    names[ConfigurableColour::Expansion_JanthirWilds] = "Expansion: Janthir Wilds";
    names[ConfigurableColour::Expansion_VisionsOfEternity] = "Expansion: Visions of Eternity";
    return names;
}

TWC::ConfigurableColourPalette TWC::ConfigurableColourPalette::Default()
{
    auto set = ConfigurableColourPalette{};
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    set[ConfigurableColour::Legendary] = rgb(153, 127, 37);
    set[ConfigurableColour::Reward] = GameDefaultColour;
    set[ConfigurableColour::Continent_Mists] = rgb(190, 200, 220);
    set[ConfigurableColour::Continent_Tyria] = rgb(150, 165, 140);
    set[ConfigurableColour::Core] = rgb(203, 59, 60);
    set[ConfigurableColour::Expansion_HeartOfThorns] = rgb(58, 113, 19);
    set[ConfigurableColour::Expansion_PathOfFire] = rgb(80, 12, 66);
    set[ConfigurableColour::Expansion_EndOfDragons] = rgb(24, 153, 166);
    set[ConfigurableColour::Expansion_SecretsOfTheObscure] = rgb(197, 158, 79);
    set[ConfigurableColour::Expansion_JanthirWilds] = rgb(34, 63, 117);
    set[ConfigurableColour::Expansion_VisionsOfEternity] = rgb(216, 114, 0);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
    return set;
}

void TWC::to_json(nlohmann::json &j, const ConfigurableColourPalette &c)
{
    for (const auto &[value, name] : magic_enum::enum_entries<ConfigurableColour>())
    {
        j[name] = c.at(value);
    }
}

void TWC::from_json(const nlohmann::json &j, ConfigurableColourPalette &c)
{
    for (const auto &[value, name] : magic_enum::enum_entries<ConfigurableColour>())
    {
        if (j.contains(name))
        {
            j.at(name).get_to(c.at(value));
        }
    }
}

template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::None>() const
{
    return convert(at(ConfigurableColour::Core));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::HeartOfThorns>() const
{
    return convert(at(ConfigurableColour::Expansion_HeartOfThorns));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::PathOfFire>() const
{
    return convert(at(ConfigurableColour::Expansion_PathOfFire));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::EndOfDragons>() const
{
    return convert(at(ConfigurableColour::Expansion_EndOfDragons));
}
template <>
GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::SecretsOfTheObscure>() const
{
    return convert(at(ConfigurableColour::Expansion_SecretsOfTheObscure));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::JanthirWilds>() const
{
    return convert(at(ConfigurableColour::Expansion_JanthirWilds));
}
template <>
GW2RE::Colour4 TWC::ConfigurableColourPalette::ExpansionCompletion<TWC::Expansion::VisionsOfEternity>() const
{
    return convert(at(ConfigurableColour::Expansion_VisionsOfEternity));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ContinentCompletion<TWC::Continent::Tyria>() const
{
    return convert(at(ConfigurableColour::Continent_Tyria));
}
template <> GW2RE::Colour4 TWC::ConfigurableColourPalette::ContinentCompletion<TWC::Continent::Mists>() const
{
    return convert(at(ConfigurableColour::Continent_Mists));
}
GW2RE::Colour4 TWC::ConfigurableColourPalette::RewardCompletion() const
{
    return convert(at(ConfigurableColour::Reward));
}
GW2RE::Colour4 TWC::ConfigurableColourPalette::LegendaryCompletion() const
{
    return convert(at(ConfigurableColour::Legendary));
}
