#pragma once
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace TWC
{
struct IdInclusion
{
    uint32_t ID;
    const char *Name;
    bool Active = true;
};

enum WorldCompletionMode : int
{
    CombinesAllMaps,
    CombinesAllExplorableMaps,
    SeparatesContinents,
    SeparatesContinentsAndExpansions,
    Chronological,
    SeparatesRegions
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

class Options
{
  public:
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

    ~Options();
    Options(std::filesystem::path filepath);
    void Persist() const;
    static void Render();

  private:
    std::filesystem::path _filepath;
};
} // namespace TWC
