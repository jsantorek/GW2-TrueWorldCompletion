#pragma once
#include "model/Colour.hpp"
#include "model/MapClassification.hpp"
#include <cstdint>
#include <optional>
#include <unordered_set>

namespace TWC
{
struct WorldCompletionConfig
{
    std::unordered_set<uint32_t> MapExclusions = {};
    std::unordered_set<uint32_t> RegionExclusions = {};

    MapClassification::WorldDivisionMode MapSeparation = MapClassification::AllMapsCollectively;

    MapClassification::ExpansionAssignmentMode ExpansionAssignment = MapClassification::MapAccessibility;

    bool IncludeMapsWithoutCompletionReward = true;
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
