#include "Workarounds.hpp"
#include "model/MapInclusion.hpp"
#include <array>

std::unordered_set<uint32_t> TWC::Workarounds::GetPoiExclusions(uint32_t id)
{ // map
    switch (id)
    {
    case 36: // Ascalonian Catacombs
        return {
            // unavailable
            942 // https://wiki.guildwars2.com/wiki/Kasha%27s_Tomb_Waypoint
        };
    case 968: // Edge of the Mists
        return {
            // unavailable
            3204 // https://wiki.guildwars2.com/wiki/Mists_Arena#Mists_Arena_Spectator_Rally_Point

        };
    case 111: // Victory or Death
        return {
            // unlockable only after breaking out of bounds
            863 // https://wiki.guildwars2.com/wiki/Zhaitan%27s_Last_Stand

        };
    case 1413: // The Twisted Marionette (Public)
        return {
            // duplicates from Lornar's Pass
            283, // https://wiki.guildwars2.com/wiki/Winterthaw_Snowfield#Winterthaw_Waypoint
            282  // https://wiki.guildwars2.com/wiki/False_River_Valley#False_River_Waypoint

        };
    case 1477: // The Tower of Nightmares (Private Squad)
        return {
            // duplicate from The Tower of Nightmares (Public)
            1839 // https://wiki.guildwars2.com/wiki/First_Level#Tower_of_Nightmares_Waypoint

        };
    case 943: // The Tower of Nightmares (Public)
        return std::unordered_set<uint32_t>{
            // duplicate from The Tower of Nightmares (Private Squad)
            3457 // https://wiki.guildwars2.com/wiki/First_Level#Tower_of_Nightmares_Waypoint

        };
    case 1483: // Memory of Old Lion's Arch
        return {
            // duplicates from Lions Arch
            1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063,
            1064, 1065, 1066, 1067, 1068, 1189, 1190, 1284, 1459, 1758, 1759,

        };
    case 1052: // Verdant Brink
        return {
            // unavailable
            1984 // Shipwreck Peak Waypoint
        };
    default:
        return {};
    }
}

std::optional<uint32_t> TWC::Workarounds::GetMapIdReplacement(uint32_t id)
{
    switch (id)
    {
        /* clang-format off */

        // Story Mode dungeons should use Exploration Mode maps' completion
        case   36:
        case   33: return   36; // Ascalonian Catacombs
        case   64:
        case   63: return   64; // Sorrow's Embrace
        case   67:
        case   68: return   67; // Twilight Arbor
        case   69:
        case   66: return   69; // Citadel of Flame
        case   71:
        case   70: return   71; // Honor of the Waves
        case   76:
        case   75: return   76; // Caudecus's Manor
        case   82:
        case   81: return   82; // Crucible of Eternity

        // The Tower of Nightmares (Public) and (Private Squad) map variants each hold
        // two same waypoints, so it is enough to use either, but not both
        case 943:
        case 1477: return 943;

        // Verdant Brink's duplicate should use regular map's completion
        case 1042: return 1052;

        // Eye of the North accessible from Hall of Monuments Portal Stone should use
        // Icebrood's Saga Eye of the North's completion
        case 1370:
        case  807: return 1370;

        // The Twisted Marionette (Private Squad) should use
        // The Twisted Marionette (Public)'s completion
        case 1414: return 1413;
        
        // All of the Mount Balrior copies should use
        // Mount Balriors' completion
        case 1564:
        case 1566:              // Map label mouse hover
        case 1562:              // Convergence: Mount Balrior (Private Squad)
        case 1571:              // Convergence: Mount Balrior (Public)
        case 1572: return 1564; // Balrior Peak: Mount Balrior

        /* clang-format on */

    default:
        return std::nullopt;
    }
}

constexpr std::array<TWC::MapInclusion, 24> MapInclusions{{
    {TWC::MapCategory::GemstoreExclusive, 1206, "Mistlock Sanctuary"},
    {TWC::MapCategory::GemstoreExclusive, 1315, "Armistice Bastion"},
    {TWC::MapCategory::GemstoreExclusive, 1465, "Thousand Seas Pavilion"},
    {TWC::MapCategory::StoryExclusive, 335, "Claw Island"},
    {TWC::MapCategory::StoryExclusive, 805, "Cathedral of HiddenDepths"},
    {TWC::MapCategory::StoryExclusive, 111, "Victory or Death"},
    {TWC::MapCategory::Historical, 901, "Molten Furnace"},
    {TWC::MapCategory::Historical, 915, "Aetherblade Retreat"},
    {TWC::MapCategory::Historical, 938, "The Reliquary"},
    {TWC::MapCategory::Historical, 939, "Resealing the Bloody Prince"},
    {TWC::MapCategory::Historical, 880, "Toypocalypse - retired"},
    {TWC::MapCategory::Seasonal_Wintersday, 877, "Snowball Mayhem"},
    {TWC::MapCategory::Seasonal_Wintersday, 878, "Tixxs Infinirarium"},
    {TWC::MapCategory::Seasonal_Wintersday, 1270, "Toypocalypse"},
    {TWC::MapCategory::Seasonal_MadKingsRealm, 862, "Reapers Rumble"},
    {TWC::MapCategory::Seasonal_MadKingsRealm, 863, "Ascent to Madness"},
    {TWC::MapCategory::Seasonal_MadKingsRealm, 866, "Mad King's Labyrinth"},
    {TWC::MapCategory::Seasonal_MadKingsRealm, 1304, "Mad King's Raceway"},
    {TWC::MapCategory::Seasonal_MadKingsRealm, 1316, "Mists Rift"},
    {TWC::MapCategory::Seasonal_LunarNewYear, 911, "Dragon Ball Arena"},
    {TWC::MapCategory::Seasonal_FestivalOfTheFourWinds, 918, "Aspect Arena"},
    {TWC::MapCategory::Seasonal_FestivalOfTheFourWinds, 922, "Labyrinthine Cliffs"},
    {TWC::MapCategory::Seasonal_FestivalOfTheFourWinds, 929, "The Crown Pavilion"},
    {TWC::MapCategory::Seasonal_SuperAdventureBox, 935, "Hub"},
}};

bool TWC::Workarounds::IsMapExplicitlyExcludable(uint32_t id)
{
    for (const auto &entry : MapInclusions)
        if (entry.ID == id)
            return true;
    return false;
}

namespace
{
constexpr auto MapCategoryToString(TWC::MapCategory cat)
{
    switch (cat)
    {
    case TWC::MapCategory::GemstoreExclusive:
        return "Gemstore exclusive";
    case TWC::MapCategory::StoryExclusive:
        return "Story exclusive";
    case TWC::MapCategory::Historical:
        return "Historical";
    case TWC::MapCategory::Seasonal_Wintersday:
        return "Seasonal: Wintersday";
    case TWC::MapCategory::Seasonal_MadKingsRealm:
        return "Seasonal: Halloween";
    case TWC::MapCategory::Seasonal_LunarNewYear:
        return "Seasonal: Lunar New Year";
    case TWC::MapCategory::Seasonal_FestivalOfTheFourWinds:
        return "Seasonal: Festival of The Four Winds";
    case TWC::MapCategory::Seasonal_SuperAdventureBox:
        return "Seasonal: Super Adventure Box";
    default:
        return "Unknown";
    }
}
}; // namespace

std::map<const char *, std::vector<std::pair<uint32_t, const char *>>> TWC::Workarounds::
    GetCategorizedExplicitlyExcludableMaps()
{
    auto inclusions = std::map<const char *, std::vector<std::pair<uint32_t, const char *>>>();
    for (uint8_t i = 0; i < static_cast<uint8_t>(TWC::MapCategory::Count); i++)
    {
        auto maps = std::vector<std::pair<uint32_t, const char *>>();
        for (const auto &entry : MapInclusions)
            if (entry.Category == static_cast<TWC::MapCategory>(i))
                maps.emplace_back(entry.ID, entry.Name);
        inclusions.emplace(MapCategoryToString(static_cast<TWC::MapCategory>(i)), std::move(maps));
    }
    return inclusions;
}
