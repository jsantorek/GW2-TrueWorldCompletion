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
        std::set<uint32_t> Landmarks = {942, 3204, 863, 1984, 1835, 1836, 1799, 1800, 1801, 1807, 1808, 1810, 1763};
        std::set<uint32_t> Tasks = {414};
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
};
} // namespace TWC
