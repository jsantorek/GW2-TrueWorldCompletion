#pragma once
#include "Configurable/Colour.hpp"
#include "Game/Types.h"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include <array>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
struct ConfigurableColourPalette : public magic_enum::containers::array<ConfigurableColour, std::array<float, 4>>
{
    static ConfigurableColourPalette Default();
    void Render();
    template <Expansion T> GW2RE::Colour4 ExpansionCompletion() const;
    template <Continent T> GW2RE::Colour4 ContinentCompletion() const;
    GW2RE::Colour4 RewardCompletion() const;
    GW2RE::Colour4 LegendaryCompletion() const;

  private:
    static magic_enum::containers::array<ConfigurableColour, std::string_view> MakeColourNames();
};
void to_json(nlohmann::json &, const ConfigurableColourPalette &);
void from_json(const nlohmann::json &, ConfigurableColourPalette &);
} // namespace TWC