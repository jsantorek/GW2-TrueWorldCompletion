#pragma once
#include "model/ContentDescriptor.hpp"
#include <Nexus.h>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace TWC
{
class ExclusionsConfigurator;
class Options
{
  public:
    void Apply() const;
    void Save() const;
    static std::unique_ptr<Options> Load();

    static void SetupConfiguration(AddonAPI *api);
    static void CleanupConfiguration(AddonAPI *api);

  private:
    static void RenderConfiguration();

  public:
    enum MissingMapsHintMode
    {
        AllIncompleteZonesOpenedInApiLink,
        OneIncompleteZoneOpenedInWiki,
        OneIncompleteZoneCenteredOnWorldMap
    } MissingMapsHint = OneIncompleteZoneCenteredOnWorldMap;
    enum CompletionMode
    {
        AllMapsCollectively,
        AllMapsWithCompletionReward,
        CurrentOrEarlierExpansionMaps,
        CurrentContinentMapsOnly,
        CurrentExpansionMapsOnly
    } WorldMap = AllMapsCollectively, CharacterSelection = AllMapsCollectively;
    enum ExpansionAssignmentMode
    {
        MapAccessibility,
        MapReleaseChronology
    } ExpansionAssignment = MapReleaseChronology;
    struct ContentExclusion
    {
        std::set<uint32_t> Landmarks = {
            942, // inaccessible WP in AC(story) https://wiki.guildwars2.com/wiki/Votive_Cathedral
            3204, // invisible, randomly unlocked WP in https://wiki.guildwars2.com/wiki/Mists_Arena
            863, // inaccessible with regular means PoI https://wiki.guildwars2.com/wiki/Zhaitan%27s_Last_Stand 
            1984, // inaccessible WP in Verdant Brink duplicate
            1835, // historical WP in Resealing the Bloody Prince
            1836, // historical WP in The Reliquary
            1799, // historical WP in Molten Furnace
            1800, // historical WP in Molten Furnace
            1801, // historical WP in Molten Furnace
            1807, // historical WP in Aetherblade Retreat
            1808, // historical WP in Aetherblade Retreat
            1810, // historical WP in Aetherblade Retreat
            1763 // historical WP in retired variant of Toypocalypse
        };
        std::set<uint32_t> Tasks = {
            414 // story exclusive duplicate of https://wiki.guildwars2.com/wiki/Help_Aksim_care_for_and_train_raptors
        };
        std::set<std::string> SkillChallanges = {};
    } Exclusion;
    struct ProgressBarColours : public std::unordered_map<ContentFeature, std::array<float, 4>>
    {
        using UnderlyingCollection = std::unordered_map<ContentFeature, std::array<float, 4>>;
        static consteval UnderlyingCollection::mapped_type rgb(uint8_t r, uint8_t g, uint8_t b)
        {
            return {static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, 1.f};
        }
        static constexpr UnderlyingCollection::mapped_type Default{0.192f, 0.545f, 0.737f, 1.0f}; // rgb(49, 139, 188);
        ProgressBarColours()
            : UnderlyingCollection(std::initializer_list<UnderlyingCollection::value_type>({
                  {ContentFeature::EXPANSION_None, rgb(203, 59, 60)},
                  {ContentFeature::EXPANSION_HeartOfThorns, rgb(58, 113, 19)},
                  {ContentFeature::EXPANSION_PathOfFire, rgb(80, 12, 66)},
                  {ContentFeature::EXPANSION_EndOfDragons, rgb(24, 153, 166)},
                  {ContentFeature::EXPANSION_SecretsOfTheObscure, rgb(197, 158, 79)},
                  {ContentFeature::EXPANSION_JanthirWilds, rgb(34, 63, 117)},
                  {ContentFeature::EXPANSION_VisionsOfEternity, rgb(216, 114, 0)},
                  {ContentFeature::CONTINENT_TheMists, rgb(132, 129, 126)},
                  {static_cast<ContentFeature>(DescriptorMask::Everything().underlying_value()), rgb(153, 127, 37)},
              }))
        {
        }
    } Colours;
    bool ShowProgressBarInAllDiscoveryAlerts = true;
    bool ShowCompletionInfoInAllLoadingScreens = true;
};
} // namespace TWC
