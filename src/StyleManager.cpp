#include "StyleManager.hpp"
#include "Strings.hpp"
#include "model/ContentDescriptor.hpp"

namespace
{
static constexpr GW2RE::Colour4 rgba(const float *rgba_f)
{
    uint8_t rgba[4];
    for (auto i = 0; i < 4; i++)
    {
        float col = rgba_f[i];
        if (col > 1.)
            col = 1.;
        else if (col < 0.)
            col = 0.;
        rgba[i] = static_cast<uint8_t>(0xFF * col);
    }
    return GW2RE::Colour4(rgba[2], rgba[1], rgba[0], rgba[3]);
}
} // namespace

TWC::StyleManager::StyleManager()
{
    WorldProgressBar = [](ContentDescriptor) { return std::make_tuple(0xBB8F, Unknown); };
}
void TWC::StyleManager::Update(const std::unordered_map<ContentFeature, std::array<float, 4>> &colours,
                               Options::ExpansionAssignmentMode exp, Options::CompletionMode wrld)
{
    CoreMask = ContentDescriptor(ContentFeature::EXPANSION_None);
    if (exp == Options::ExpansionAssignmentMode::MapAccessibility)
        CoreMask |= ContentFeature::EXPANSION_Optional;
    Core = rgba(colours.at(ContentFeature::EXPANSION_None).data());
    HeartOfThorns = rgba(colours.at(ContentFeature::EXPANSION_HeartOfThorns).data());
    PathOfFire = rgba(colours.at(ContentFeature::EXPANSION_PathOfFire).data());
    EndOfDragons = rgba(colours.at(ContentFeature::EXPANSION_EndOfDragons).data());
    SecretsOfTheObscure = rgba(colours.at(ContentFeature::EXPANSION_SecretsOfTheObscure).data());
    JanthirWilds = rgba(colours.at(ContentFeature::EXPANSION_JanthirWilds).data());
    VisionsOfEternity = rgba(colours.at(ContentFeature::EXPANSION_VisionsOfEternity).data());
    switch (wrld)
    {
    case Options::AllMapsCollectively:
    case Options::CurrentOrEarlierExpansionMaps:
    case Options::AllMapsWithCompletionReward:
        WorldProgressBar =
            [col =
                 rgba(colours.at(static_cast<ContentFeature>(DescriptorMask::Everything().underlying_value())).data())](
                ContentDescriptor) { return std::make_tuple(TWC::Strings::Legendary, col); };
        break;
    case Options::CurrentContinentMapsOnly:
        WorldProgressBar = [&,
                            mists = rgba(colours.at(ContentFeature::CONTINENT_TheMists).data())](ContentDescriptor d) {
            d &= DescriptorMask::Continents();
            switch (static_cast<ContentFeature>(d.underlying_value()))
            {
            case ContentFeature::CONTINENT_TheMists:
                return std::make_tuple(Strings::TheMists, mists);
            case ContentFeature::CONTINENT_Tyria:
                return std::make_tuple(Strings::Tyria, Core);
            default:
                break;
            }
            return std::make_tuple(Strings::QuestionMarks, Unknown);
        };
        break;
    case Options::CurrentExpansionMapsOnly:
        WorldProgressBar = [&](ContentDescriptor d) { return GetExpansionStyle(d); };
        break;
    }
}

std::tuple<TWC::Strings::Identifier, GW2RE::Colour4> TWC::StyleManager::GetExpansionStyle(ContentDescriptor dscr)
{
    if (dscr & CoreMask)
        return {Strings::CentralTyria, Core};

    dscr &= DescriptorMask::Expansions();
    switch (static_cast<ContentFeature>(dscr.underlying_value()))
    {
    case ContentFeature::EXPANSION_HeartOfThorns:
        return {Strings::HeartOfThorns, HeartOfThorns};
    case ContentFeature::EXPANSION_PathOfFire:
        return {Strings::PathOfFire, PathOfFire};
    case ContentFeature::EXPANSION_EndOfDragons:
        return {Strings::EndOfDragons, EndOfDragons};
    case ContentFeature::EXPANSION_SecretsOfTheObscure:
        return {Strings::SecretsOfTheObscure, SecretsOfTheObscure};
    case ContentFeature::EXPANSION_JanthirWilds:
        return {Strings::JanthirWilds, JanthirWilds};
    case ContentFeature::EXPANSION_VisionsOfEternity:
        return {Strings::QuestionMarks, VisionsOfEternity}; // TODO
    default:
        break;
    }
    return {Strings::QuestionMarks, Unknown};
}
