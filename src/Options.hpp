#pragma once
#include <filesystem>
#include <string_view>
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
    Tyria = 1,
    TheMists = 2,
    Core = 3,
    HeartOfThorns = 4,
    PathOfFire = 5,
    EndOfDragons = 6,
    SecretsOfTheObscure = 7,
    JanthirWilds = 8,
    ToBeDetermined
};

constexpr std::string_view to_string(ContinentsAndExpansionsEnumeration e)
{
    switch (e)
    {
    case Undetermined:
        return "Undetermined";
    case TheMists:
        return "The Mists";
    case Tyria:
        return "Tyria";
    case Core:
        return "Central Tyria";
    case HeartOfThorns:
        return "Heart of Thorns";
    case PathOfFire:
        return "Path of Fire";
    case EndOfDragons:
        return "End of Dragons";
    case SecretsOfTheObscure:
        return "Secrets of the Obscure";
    case JanthirWilds:
        return "Janthir Wilds";
    default:
        break;
    }
    return "?";
}
constexpr int to_int(ContinentsAndExpansionsEnumeration e)
{
    return static_cast<int>(e);
}

class Options
{
  public:
    int WorldCompletion = CombinesAllMaps;

    std::unordered_map<ContinentsAndExpansionsEnumeration, IdInclusion> ContinentsAndExpansionsInclusions = {
        {Tyria, IdInclusion{Tyria, to_string(Tyria).data()}},
        {TheMists, IdInclusion{TheMists, to_string(TheMists).data()}},
        {Core, IdInclusion{Core, to_string(Core).data()}},
        {HeartOfThorns, IdInclusion{HeartOfThorns, to_string(HeartOfThorns).data()}},
        {PathOfFire, IdInclusion{PathOfFire, to_string(PathOfFire).data()}},
        {EndOfDragons, IdInclusion{EndOfDragons, to_string(EndOfDragons).data()}},
        {SecretsOfTheObscure, IdInclusion{SecretsOfTheObscure, to_string(SecretsOfTheObscure).data()}},
        {JanthirWilds, IdInclusion{JanthirWilds, to_string(JanthirWilds).data()}},
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
