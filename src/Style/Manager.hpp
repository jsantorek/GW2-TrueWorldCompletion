#pragma once
#include "Completion/Byte.hpp"
#include "Completion/Store.hpp"
#include "Completion/Value.hpp"
#include "Configurable/ColourPalette.hpp"
#include "Configurable/TextFormat.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Game/Types.h"
#include "Map/Descriptor.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include "Style/Definition.hpp"
#include <magic_enum/magic_enum.hpp>
#include <mutex>

namespace TWC
{
class StyleManager
{
  public:
    void Update(const ConfigurableColourPalette &, ConfigurableExpansionAssignment, ConfigurableWorldCompletion,
                const ConfigurableTextFormat &);
    StyleDefinition::MapProgress GetMapProgress(TWC::MapDescriptor) const;
    StyleDefinition::WorldProgress GetWorldProgress(CompletionValue) const;
    StyleDefinition::CharacterProgress GetCharacterProgress(std::string_view, uint32_t, CompletionByte) const;

  private:
    GW2RE::Colour4 GetExpansionColour(MapDescriptor) const;
    GW2RE::Colour4 GetWorldColour(uint32_t, uint32_t) const;
    std::tuple<GW2RE::Colour4, std::string> GetCurrentWorldColourAndName() const;
    std::string SerializeGiftOfExplorationProgress(CompletionByte) const;
    std::string SerializeCompletionProgress(const LocalizedCompletionStore<CompletionValue> &) const;
    std::string GetCharacterProgressText(std::optional<CompletionValue>, CompletionByte) const;

    std::mutex Mutex;
    ConfigurableExpansionAssignment Assignment = ConfigurableExpansionAssignment::Default;
    ConfigurableWorldCompletion Completion = ConfigurableWorldCompletion::Default;
    FormatCharacterProgressText CharacterProgressText = FormatCharacterProgressText::Default;
    FormatCharacterProgressTooltip CharacterProgressTooltip = FormatCharacterProgressTooltip::Default;
    FormatWorldMapProgress WorldMapProgress = FormatWorldMapProgress::Default;

    GW2RE::Colour4 LegendaryColour = {};
    GW2RE::Colour4 RewardColour = {};
    GW2RE::Colour4 MistsColour = {};
    static constexpr GW2RE::Colour4 UnknownColour = GW2RE::Colour4::rgb(0, 0, 0);
    magic_enum::containers::array<TWC::Expansion, GW2RE::Colour4> ExpansionColours = {};
    magic_enum::containers::array<TWC::Continent, GW2RE::Colour4> ContinentColours = {};
};
} // namespace TWC
namespace G
{
extern TWC::StyleManager *Style;
}