#include "Style/Manager.hpp"
#include "Completion/Byte.hpp"
#include "Completion/Cache.hpp"
#include "Completion/Store.hpp"
#include "Completion/Value.hpp"
#include "Configurable/ColourPalette.hpp"
#include "Configurable/ExpansionAssignment.hpp"
#include "Configurable/TextFormat.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Format/CharacterProgressText.hpp"
#include "Format/CharacterProgressTooltip.hpp"
#include "Format/WorldMapProgress.hpp"
#include "Game/Types.h"
#include "Map/Descriptor.hpp"
#include "Map/Utils.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include "Style/Definition.hpp"
#include "Text/Localization.hpp"
#include "Text/Tag.hpp"
#include "Text/Utilities.hpp"
#include <Logging.hpp>
#include <cstdint>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <mutex>
#include <optional>
#include <string_view>
#include <tuple>
#include <utility>

void TWC::StyleManager::Update(const ConfigurableColourPalette &colours, ConfigurableExpansionAssignment assignment,
                               ConfigurableWorldCompletion completion, const ConfigurableTextFormat &format)
{
    const auto lock = std::lock_guard(Mutex);
    Assignment = assignment;
    Completion = completion;
    CharacterProgressText = format.CharacterProgressText;
    CharacterProgressTooltip = format.CharacterProgressTooltip;
    WorldMapProgress = format.WorldMapProgress;

    LegendaryColour = colours.LegendaryCompletion();
    RewardColour = colours.RewardCompletion();
    ContinentColours[Continent::None] = UnknownColour;
    ContinentColours[Continent::Mists] = colours.ContinentCompletion<Continent::Mists>();
    ContinentColours[Continent::Tyria] = colours.ContinentCompletion<Continent::Tyria>();
    ExpansionColours[Expansion::None] = colours.ExpansionCompletion<Expansion::None>();
    ExpansionColours[Expansion::HeartOfThorns] = colours.ExpansionCompletion<Expansion::HeartOfThorns>();
    ExpansionColours[Expansion::PathOfFire] = colours.ExpansionCompletion<Expansion::PathOfFire>();
    ExpansionColours[Expansion::EndOfDragons] = colours.ExpansionCompletion<Expansion::EndOfDragons>();
    ExpansionColours[Expansion::SecretsOfTheObscure] = colours.ExpansionCompletion<Expansion::SecretsOfTheObscure>();
    ExpansionColours[Expansion::JanthirWilds] = colours.ExpansionCompletion<Expansion::JanthirWilds>();
    ExpansionColours[Expansion::VisionsOfEternity] = colours.ExpansionCompletion<Expansion::VisionsOfEternity>();
}

TWC::StyleDefinition::MapProgress TWC::StyleManager::GetMapProgress(TWC::MapDescriptor dscr) const
{
    auto style = StyleDefinition::MapProgress{};
    switch (Assignment)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received, falling back to default {}",
            magic_enum::enum_type_name<ConfigurableExpansionAssignment>(), magic_enum::enum_name(Assignment),
            magic_enum::enum_name(ConfigurableExpansionAssignment::AccessabilityBased));
    case ConfigurableExpansionAssignment::AccessabilityBased:
        style.Colour = ExpansionColours[dscr.GetExpansion<ConfigurableExpansionAssignment::AccessabilityBased>()];
        break;
    case ConfigurableExpansionAssignment::ChronologyBased:
        style.Colour = ExpansionColours[dscr.GetExpansion<ConfigurableExpansionAssignment::ChronologyBased>()];
        break;
    }
    return style;
}

TWC::StyleDefinition::WorldProgress TWC::StyleManager::GetWorldProgress(CompletionValue value) const
{
    auto [colour, name] = GetCurrentWorldColourAndName();
    auto text = std::string{};
    switch (WorldMapProgress)
    {
    case FormatWorldMapProgress::ShowValues:
        text = std::format("{}/{}", value.Completed, value.Available);
        break;
    case FormatWorldMapProgress::ShowPercentage:
        text = std::format("{:.1f}%", value.Percent());
        break;
    }
    return TWC::StyleDefinition::WorldProgress{
        .Text = std::move(text),
        .Label = std::move(name),
        .Colour = std::move(colour),
    };
}

GW2RE::Colour4 TWC::StyleManager::GetWorldColour(uint32_t id1, uint32_t id2) const
{
    switch (Completion)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received", magic_enum::enum_type_name<ConfigurableWorldCompletion>(),
            magic_enum::enum_name(Completion));
        break;
    case ConfigurableWorldCompletion::AllMapsCollectively:
        return LegendaryColour;
    case ConfigurableWorldCompletion::AllMapsWithCompletionReward:
        return RewardColour;
    case ConfigurableWorldCompletion::CurrentContinentMapsOnly: {
        if (auto continent = MapUtils::MatchContinent(id1, id2))
        {
            return ContinentColours[continent.value()];
        }
        break;
    }
    case ConfigurableWorldCompletion::CurrentExpansionMapsOnly:
    case ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps: {
        if (auto expansion = MapUtils::MatchExpansions(Assignment, id1, id2))
        {
            return ExpansionColours[expansion.value()];
        }
        break;
    }
    }
    return UnknownColour;
}
TWC::StyleDefinition::CharacterProgress TWC::StyleManager::GetCharacterProgress(std::string_view characterName,
                                                                                uint32_t lastMapId,
                                                                                CompletionByte origin) const
{
    auto style = StyleDefinition::CharacterProgress{};
    const auto value = G::Cache::Completion->GetCompletion(characterName);
    if (value)
    {
        auto colour = GetWorldColour(lastMapId, value->MapId);
        style.Text = std::format("<c=#{:02X}{:02X}{:02X}>", colour.R, colour.G, colour.B);
        if (CharacterProgressText.test(FormatCharacterProgressTextFlags::ShowWorldValues))
        {
            style.Text.append(std::format("[{}/{}]", value->World.Completed, value->World.Available));
        }
        if (CharacterProgressText.test(FormatCharacterProgressTextFlags::ShowWorldPercentage))
        {
            style.Text.append(std::format(" {:.1f}%", value->World.Percent()));
        }
        style.Text.append("</c>");
        if (CharacterProgressTooltip.IsCustomized())
            style.Tooltip =
                std::format("True World Completion<br> <br>{}<br>{}:", SerializeCompletionProgress(value.value()),
                            TextLocalization::FetchDescription<TextTag::GiftOfExploration>());
    }
    else
    {
        style.Text = TextUtilities::Uncached;
        if (CharacterProgressTooltip.IsCustomized())
            style.Tooltip = std::format("True World Completion<br> <br>{} {}!<br>{}:", characterName,
                                        TextLocalization::FetchDescription<TextTag::CharacterUncachedExplanation>(),
                                        TextLocalization::FetchDescription<TextTag::GiftOfExploration>());
    }
    if (style.Tooltip)
    {
        if (CharacterProgressTooltip.test(FormatCharacterProgressTooltipFlags::ShowValues))
        {
            style.Tooltip->append(std::format(" [{}/255]", origin.Completed));
        }
        if (CharacterProgressTooltip.test(FormatCharacterProgressTooltipFlags::ShowPercentages))
        {
            style.Tooltip->append(std::format(" {:.1f}%", origin.Percent()));
        }
    }

    if (CharacterProgressText.test(FormatCharacterProgressTextFlags::ShowGiftOfBattleValues))
    {
        style.Text.append(std::format(" [{}/255]", origin.Completed));
    }
    if (CharacterProgressText.test(FormatCharacterProgressTextFlags::ShowGiftOfBattlePercentage))
    {
        style.Text.append(std::format(" {:.1f}%", origin.Percent()));
    }
    return style;
}

GW2RE::Colour4 TWC::StyleManager::GetExpansionColour(MapDescriptor dscr) const
{
    switch (Assignment)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received", magic_enum::enum_type_name<ConfigurableExpansionAssignment>(),
            magic_enum::enum_name(Assignment));
        break;
    case ConfigurableExpansionAssignment::AccessabilityBased:
        return ExpansionColours[dscr.GetExpansion<ConfigurableExpansionAssignment::AccessabilityBased>()];
    case ConfigurableExpansionAssignment::ChronologyBased:
        return ExpansionColours[dscr.GetExpansion<ConfigurableExpansionAssignment::ChronologyBased>()];
    }
    return UnknownColour;
}

std::string TWC::StyleManager::SerializeCompletionProgress(const LocalizedCompletionStore<CompletionValue> &store) const
{
    auto str = std::string{};
    auto appendLine = [&](GW2RE::Colour4 colour, CompletionValue value, std::string_view name) {
        if (!value.Completed)
            return;
        str.append(std::format("<c=#{:02X}{:02X}{:02X}>{}</c>:", colour.R, colour.G, colour.B, name));
        if (CharacterProgressTooltip.test(FormatCharacterProgressTooltipFlags::ShowValues))
        {
            str.append(std::format(" [{}/{}]", value.Completed, value.Available));
        }
        if (CharacterProgressTooltip.test(FormatCharacterProgressTooltipFlags::ShowPercentages))
        {
            str.append(std::format(" {:.1f}%", value.Percent()));
        }
        str.append("<br>");
    };
    appendLine(LegendaryColour, store.Legendary,
               TextLocalization::FetchDescription<ConfigurableWorldCompletion::AllMapsCollectively>());
    appendLine(RewardColour, store.Reward,
               TextLocalization::FetchDescription<ConfigurableWorldCompletion::AllMapsWithCompletionReward>());
    for (const auto &value : magic_enum::enum_values<Expansion>())
    {
        appendLine(ExpansionColours[value], store.Expansions[value],
                   TextLocalization::FetchDescription<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(value));
    }
    return str;
}

std::tuple<GW2RE::Colour4, std::string> TWC::StyleManager::GetCurrentWorldColourAndName() const
{
    switch (Completion)
    {
    case ConfigurableWorldCompletion::AllMapsCollectively:
        return {LegendaryColour,
                TextLocalization::FetchDescription<ConfigurableWorldCompletion::AllMapsCollectively>()};
    case ConfigurableWorldCompletion::AllMapsWithCompletionReward:
        return {RewardColour,
                TextLocalization::FetchDescription<ConfigurableWorldCompletion::AllMapsWithCompletionReward>()};
    case ConfigurableWorldCompletion::CurrentContinentMapsOnly: {
        if (auto dscr = MapDescriptor::FromCurrentMap())
        {
            const auto cont = dscr->GetContinent();
            return {ContinentColours[cont],
                    TextLocalization::FetchDescription<ConfigurableWorldCompletion::CurrentContinentMapsOnly>(cont)};
        }
        break;
    }
    case ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps:
    case ConfigurableWorldCompletion::CurrentExpansionMapsOnly:
        if (auto dscr = MapDescriptor::FromCurrentMap())
        {
            return {GetExpansionColour(dscr.value()),
                    TextLocalization::FetchDescription<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(
                        dscr->GetExpansion<ConfigurableExpansionAssignment::AccessabilityBased>())};
        }
        break;
    }
    return {UnknownColour, TextUtilities::Unidentified};
}
