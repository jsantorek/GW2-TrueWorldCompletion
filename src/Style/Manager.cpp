#include "Style/Manager.hpp"
#include "Completion/Byte.hpp"
#include "Completion/Cache.hpp"
#include "Completion/Value.hpp"
#include "Game/Types.h"
#include "Map/Features.hpp"
#include "Model/Expansion.hpp"
#include <codecvt>
#include <format>
#include <magic_enum/magic_enum.hpp>

TWC::StyleDefinition::MapProgress TWC::StyleManager::GetMapProgress(TWC::MapDescriptor dscr) const
{
    if (dscr.Features.count(MapFeatures::Accessible_Festival))
    {
        return StyleDefinition::MapProgress{.Colour = GetExpansionColour(TWC::Expansion::None)};
    }
    else if (false && // TODO expansion assignment based on accessibility
             dscr.Features.count(MapFeatures::Accessible_FreeAccount))
    {
        return StyleDefinition::MapProgress{.Colour = GetExpansionColour(TWC::Expansion::None)};
    }
    else
    {
        return StyleDefinition::MapProgress{.Colour = GetExpansionColour(dscr.Expansion)};
    }
    // {Retired::ContentFeature::CONTINENT_TheMists, rgb(132, 129, 126)}, {},
}

TWC::StyleDefinition::WorldProgress TWC::StyleManager::GetWorldProgress(CompletionValue value) const
{
    auto [colour, name] = GetWorldColourAndName(0);
    return TWC::StyleDefinition::WorldProgress{
        .Text = std::format(L"{}/{}", value.Completed, value.Available),
        .Label = std::move(name),
        .Colour = std::move(colour),
    };
}

TWC::StyleDefinition::CharacterProgress TWC::StyleManager::GetCharacterProgress(std::wstring_view characterName,
                                                                                uint32_t lastMapId,
                                                                                CompletionByte origin) const
{
    if (auto value = G::Cache::CharacterInfo->GetCompletion(characterName))
    {
        if (lastMapId != value->MapId)
            ;
        return StyleDefinition::CharacterProgress{.Text = std::format(L"<c=#997f25>[{}/{}]</c> {}/255",
                                                                      value->World.Completed, value->World.Available,
                                                                      origin.Completed),
                                                  .Tooltip = std::format(L"True World Completion <br> <br>{}{}<br> ",
                                                                         SerializeCompletionProgress(value.value()),
                                                                         SerializeGiftOfExplorationProgress(origin))};
    }
    return StyleDefinition::CharacterProgress{
        .Text = std::format(L"-- {}/255", origin.Completed),
        .Tooltip = std::format(L"True World Completion<br> <br>{} data unavailable!<br>{}<br> ", characterName,
                               SerializeGiftOfExplorationProgress(origin)),
    };
}

GW2RE::Colour4 TWC::StyleManager::GetExpansionColour(TWC::Expansion expansion) const
{
    switch (expansion)
    {
    case TWC::Expansion::None:
        return GW2RE::Colour4::rgb(203, 59, 60);
    case TWC::Expansion::HeartOfThorns:
        return GW2RE::Colour4::rgb(58, 113, 19);
    case TWC::Expansion::PathOfFire:
        return GW2RE::Colour4::rgb(80, 12, 66);
    case TWC::Expansion::EndOfDragons:
        return GW2RE::Colour4::rgb(24, 153, 166);
    case TWC::Expansion::SecretsOfTheObscure:
        return GW2RE::Colour4::rgb(197, 158, 79);
    case TWC::Expansion::JanthirWilds:
        return GW2RE::Colour4::rgb(34, 63, 117);
    case TWC::Expansion::VisionsOfEternity:
        return GW2RE::Colour4::rgb(216, 114, 0);
    }
    return GW2RE::Colour4::rgb(0, 0, 0);
}

std::wstring TWC::StyleManager::SerializeGiftOfExplorationProgress(CompletionByte completion) const
{
    return std::format(L"Gift of Exploration: [{}/255] {:.1f}%", completion.Completed, 100.f * float(completion));
}

std::wstring TWC::StyleManager::SerializeCompletionProgress(const LocalizedCompletionStore<CompletionValue> &store) const
{
    auto str = std::wstring{};
    auto appendLine = [&](GW2RE::Colour4 colour, CompletionValue value, std::wstring name) {
        str.append(std::format(L"<c=#{:02X}{:02X}{:02X}>{}</c>: [{}/{}] {:.1f}%<br>", colour.R, colour.G, colour.B,
                               name, value.Completed, value.Available, 100.f * float(value)));
    };
    appendLine(GW2RE::Colour4::rgb(153, 127, 37), store.World, L"Legendary World Completion");
    appendLine(GW2RE::Colour4::rgb(49, 139, 188), store.Reward, L"Completion Rewards");
    for (const auto &[value, name] : magic_enum::enum_entries<Expansion>())
    {
        appendLine(GetExpansionColour(value), store.Expansions[static_cast<size_t>(value)],
                   std::wstring(name.begin(), name.end()));
    }
    return str;
}

std::tuple<GW2RE::Colour4, std::wstring> TWC::StyleManager::GetWorldColourAndName(uint32_t) const
{
    return {GW2RE::Colour4::rgb(153, 127, 37), L"Legendary World Completion"};
}
