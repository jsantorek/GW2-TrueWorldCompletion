#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include "Text/Localization.hpp"
#include <Nexus.h>
#include <codecvt>
#include <format>
#include <locale>
#include <magic_enum/magic_enum.hpp>
#include <string>
namespace
{
template <typename IdentifierType> auto GetIdentifier(IdentifierType Identifier)
{
    return std::format("{}::{}::{}", ADDON_ACRONYM, magic_enum::enum_type_name<IdentifierType>(),
                       magic_enum::enum_name(Identifier));
}
static constexpr auto TrueWorldCompletionIdentifier = ADDON_ACRONYM "::" ADDON_NAME;
static LOCALIZATION_TRANSLATE Translate = nullptr;
} // namespace
void TWC::TextLocalization::Initialize(const AddonAPI::LocalizationVT &localization)
{
    localization.Set(TrueWorldCompletionIdentifier, "en", "True World Completion");

    auto id = GetIdentifier(Expansion::None);
    localization.Set(id.c_str(), "en", "Central Tyria Completion");

    id = GetIdentifier(Expansion::HeartOfThorns);
    localization.Set(id.c_str(), "en", "Heart of Thorns Completion");

    id = GetIdentifier(Expansion::PathOfFire);
    localization.Set(id.c_str(), "en", "Path of Fire Completion");

    id = GetIdentifier(Expansion::EndOfDragons);
    localization.Set(id.c_str(), "en", "End of Dragons Completion");

    id = GetIdentifier(Expansion::SecretsOfTheObscure);
    localization.Set(id.c_str(), "en", "Secrets of the Obscure Completion");

    id = GetIdentifier(Expansion::JanthirWilds);
    localization.Set(id.c_str(), "en", "Janthir Wilds Completion");

    id = GetIdentifier(Expansion::VisionsOfEternity);
    localization.Set(id.c_str(), "en", "Visions of Eternity Completion");

    id = GetIdentifier(Continent::Mists);
    localization.Set(id.c_str(), "en", "The Mists Completion");

    id = GetIdentifier(Continent::Tyria);
    localization.Set(id.c_str(), "en", "Tyria Completion");

    // id = GetIdentifier(Inclusion::Completion_WorldMap);
    // localization.Set(id.c_str(), "en", "Custom World Completion");
    Translate = localization.Translate;
}

std::wstring TWC::TextLocalization::FetchDescription(Expansion str)
{
    const auto id = GetIdentifier(str);
    auto translation = Translate(id.c_str());
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(translation);
}
// std::wstring TWC::TextLocalization::FetchDescription(Inclusion str)
// {
//     auto id = magic_enum::enum_contains(str) ? GetIdentifier(str) : TrueWorldCompletionIdentifier;
//     auto translation = localization.Translate(id.c_str());
//     return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(translation);
// }
