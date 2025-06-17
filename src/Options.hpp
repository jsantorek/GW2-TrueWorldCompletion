#pragma once
#include "model/Colour.hpp"
#include "model/IdInclusion.hpp"
#include "model/MapClassification.hpp"
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace TWC
{
struct WorldCompletionConfig
{
    std::unordered_map<const char *, std::vector<IdInclusion>> MapInclusions = {
        {"Gemstore exclusive",
         {
             IdInclusion{1206, "Mistlock Sanctuary"},
             IdInclusion{1315, "Armistice Bastion"},
             IdInclusion{1465, "Thousand Seas Pavilion"},
         }},
        {"Story exclusive", {IdInclusion{335, "Claw Island"}, IdInclusion{111, "Victory or Death"}}},
        {"Historical", {IdInclusion{901, "Molten Furnace"}, IdInclusion{915, "Aetherblade Retreat"}}},
        {"Wintersday Celebration",
         {
             IdInclusion{877, "Snowball Mayhem"},
             IdInclusion{878, "Tixx's Infinirarium"},
             IdInclusion{1270, "Toypocalypse"},
         }},
        {"Mad King's Realm",
         {
             IdInclusion{862, "Reaper's Rumble"},
             IdInclusion{863, "Ascent to Madness"},
             IdInclusion{866, "Mad King's Labyrinth"},
             IdInclusion{1304, "Mad King's Raceway"},
             IdInclusion{1316, "Mists Rift"},
         }},
        {"Lunar New Year",
         {
             IdInclusion{911, "Dragon Ball Arena"},
         }},
        {"Festival of the Four Winds",
         {
             IdInclusion{918, "Aspect Arena"},
             IdInclusion{922, "Labyrinthine Cliffs"},
             IdInclusion{929, "The Crown Pavilion"},
         }},
        {"Super Adventure Box",
         {
             IdInclusion{935, "Hub"},
         }}};
    std::vector<uint32_t> MapExclusions = {};
    std::vector<uint32_t> RegionExclusions = {};
    std::vector<IdInclusion> RegionInclusions{
        IdInclusion{6, "Player vs. Player"},
        IdInclusion{7, "World vs. World"},
    };

    MapClassification::WorldDivisionMode MapSeparation = MapClassification::AllMapsCollectively;

    MapClassification::ExpansionAssignmentMode ExpansionAssignment = MapClassification::MapAccessibility;

    bool IncludeMapsWithoutCompletionReward = true;

    std::unordered_set<uint32_t> GetExcludedMapIds() const;
    std::unordered_set<uint32_t> GetExcludedRegionIds() const;
};

class Options
{
  public:
    ~Options();
    Options();

    void Persist() const;

    Colour4 GetMapProgressBarColour(MapClassification classification) const;

    Colour4 GetWorldProgressBarColour(MapClassification classification) const;

    uint32_t GetWorldProgressBarTitle(MapClassification classification) const;

    bool DefaultColorScheme = false;
    WorldCompletionConfig WorldProgressWidget = {};
    std::optional<WorldCompletionConfig> CharacterInfoWidget = std::nullopt;
    enum MissingMapsHintMode
    {
        AllIncompleteZonesOpenedInApiLink,
        OneIncompleteZoneOpenedInWiki,
        AllIncompleteZonesShownInMapCinematic,
        OneIncompleteZoneCenteredOnWorldMap
    } MissingMapsHint = OneIncompleteZoneCenteredOnWorldMap;

  private:
    static void Render();
    static void RenderWorldCompletionConfig(WorldCompletionConfig &);
};
} // namespace TWC
namespace G
{
extern TWC::Options *Options;
}
