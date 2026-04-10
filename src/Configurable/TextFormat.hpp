#pragma once
#include "Format/CharacterProgressText.hpp"
#include "Format/CharacterProgressTooltip.hpp"
#include "Format/WorldMapProgress.hpp"
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
struct ConfigurableTextFormat
{
    static ConfigurableTextFormat Default();
    void Render();
    FormatCharacterProgressText CharacterProgressText = FormatCharacterProgressText::Default;
    FormatCharacterProgressTooltip CharacterProgressTooltip = FormatCharacterProgressTooltip::Default;
    FormatWorldMapProgress WorldMapProgress = FormatWorldMapProgress::Default;
};
void to_json(nlohmann::json &, const ConfigurableTextFormat &);
void from_json(const nlohmann::json &, ConfigurableTextFormat &);
} // namespace TWC