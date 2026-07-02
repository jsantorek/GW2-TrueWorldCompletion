#include "Text/Localization.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include "Text/Tag.hpp"
#include <Nexus.h>
#include <magic_enum/magic_enum.hpp>
#include <string>

LOCALIZATION_TRANSLATE TWC::TextLocalization::Translate = nullptr;

void TWC::TextLocalization::Initialize(const AddonAPI::LocalizationVT &localization)
{
    auto id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::None);
    localization.Set(id.c_str(), "en", "Central Tyria Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::HeartOfThorns);
    localization.Set(id.c_str(), "en", "Heart of Thorns Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::PathOfFire);
    localization.Set(id.c_str(), "en", "Path of Fire Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::EndOfDragons);
    localization.Set(id.c_str(), "en", "End of Dragons Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::SecretsOfTheObscure);
    localization.Set(id.c_str(), "en", "Secrets of the Obscure Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::JanthirWilds);
    localization.Set(id.c_str(), "en", "Janthir Wilds Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentExpansionMapsOnly>(Expansion::VisionsOfEternity);
    localization.Set(id.c_str(), "en", "Visions of Eternity Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::None);
    localization.Set(id.c_str(), "en", "Core Game Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::HeartOfThorns);
    localization.Set(id.c_str(), "en", "Heart of Thorns Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::PathOfFire);
    localization.Set(id.c_str(), "en", "Path of Fire Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::EndOfDragons);
    localization.Set(id.c_str(), "en", "End of Dragons Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::SecretsOfTheObscure);
    localization.Set(id.c_str(), "en", "Secrets of the Obscure Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::JanthirWilds);
    localization.Set(id.c_str(), "en", "Janthir Wilds Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps>(Expansion::VisionsOfEternity);
    localization.Set(id.c_str(), "en", "Visions of Eternity Era Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentContinentMapsOnly>(Continent::Mists);
    localization.Set(id.c_str(), "en", "The Mists Completion");

    id = GetIdentifier<ConfigurableWorldCompletion::CurrentContinentMapsOnly>(Continent::Tyria);
    localization.Set(id.c_str(), "en", "Tyria Completion");

    id = GetIdentifier<TextTag::GiftOfExploration>();
    localization.Set(id.c_str(), "en", "Gift of Exploration");

    id = GetIdentifier<TextTag::CharacterUncachedExplanation>();
    localization.Set(id.c_str(), "en", "data unavailable.\nLog into the game to update it");

    id = GetIdentifier<ConfigurableWorldCompletion::AllMapsWithCompletionReward>();
    localization.Set(id.c_str(), "en", "Completion Rewards");

    id = GetIdentifier<ConfigurableWorldCompletion::AllMapsCollectively>();
    localization.Set(id.c_str(), "en", "Legendary World Completion");

    Translate = localization.Translate;
}

std::string TWC::TextLocalization::FetchValidated(std::string id)
{
    const auto str = Translate(id.c_str());
    if (str)
    {
        auto res = std::string{str};
        if (!res.empty())
        {
            return res;
        }
    }
    return TextUtilities::Unlocalized;
}