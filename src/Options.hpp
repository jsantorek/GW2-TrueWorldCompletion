#pragma once
#include <filesystem>
#include <unordered_map>
#include <vector>
namespace TWC
{
struct IdInclusion
{
    int ID;
    const char *Name;
    bool Active = true;
};
/*
World completion: (unmodded: combines all maps required for Gift of Exploration)
Combines all maps
Separates Mists and Tyria
Separates Mists and each expansion cycle on Tyria
Excludes expansion cycles subsequent to current map
Excludes maps without completion reward

World completion is displayed: (unmodded: when on map required for Gift of Exploration)
Always
If current map contributes to World completion

Map completion is displayed for maps: (unmodded: with completion reward)
with discoverable points
contributing to World completion

Explicitly include maps:
//
Explicitly include pois:

*/
enum WorldCompletionMode : int
{
    CombinesAllMaps,
    CombinesAllExplorableMaps,
    SeparatesContinents,
    SeparatesContinentsAndExpansions,
    Chronological,

    Count
};
enum ContinentsAndExpansionsEnumeration : uint32_t
{
    Undetermined = 0,
    TheMists = 1,
    Tyria = 2,
    Core = 3,
    HearOfThorns = 4,
    PathOfFire = 5,
    EndOfDragons = 6,
    SecretsOfTheObscure = 7,
    JanthirWilds = 8,
    ToBeDetermined
};

struct Options
{
    int WorldCompletion = CombinesAllMaps;

    std::unordered_map<ContinentsAndExpansionsEnumeration, IdInclusion> ContinentsAndExpansionsInclusions = {
        {Undetermined, IdInclusion{Undetermined, "Undetermined", false}},
        {Tyria, IdInclusion{Tyria, "Tyria"}},
        {TheMists, IdInclusion{TheMists, "The Mists"}},
        {Core, IdInclusion{Core, "Core"}},
        {HearOfThorns, IdInclusion{HearOfThorns, "Heart of Thorns"}},
        {PathOfFire, IdInclusion{PathOfFire, "Path of Fire"}},
        {EndOfDragons, IdInclusion{EndOfDragons, "End of Dragons"}},
        {SecretsOfTheObscure, IdInclusion{SecretsOfTheObscure, "Secrets of the Obscure"}},
        {JanthirWilds, IdInclusion{JanthirWilds, "Janthir Wilds"}},
    };
    std::unordered_map<const char *, std::vector<IdInclusion>> MapInclusions = {
        {"Gemstore exclusive",
         {
             IdInclusion{1315, "Armistice Bastion"},
             IdInclusion{1465, "Thousand Seas Pavilion"},
         }},
        {"Story exclusive", {IdInclusion{335, "Claw Island"}}},
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
         }}};

    void Persist(std::filesystem::path filepath) const;
    void Parse(std::filesystem::path filepath);
};
} // namespace TWC
