#include "Workarounds.hpp"

/*
TODO:
901 Molten Furnace
915 Aetherblade Retreat
    some LWS1 stuff...? can it be discovered in fractals?
*/

std::optional<TWC::CustomCompletion> TWC::Workarounds::GetCustomCompletion(uint32_t id)
{
    switch (id)
    {
    case 36: // Ascalonian Catacombs
        return CustomCompletion{.BaseMaps =
                                    {
                                        33, // Ascalonian Catacombs (Story)
                                        36  // Ascalonian Catacombs (Explorable)
                                    },
                                .PoiExclusions = {
                                    942 // https://wiki.guildwars2.com/wiki/Kasha%27s_Tomb_Waypoint
                                }};
    case 67: // Twilight Arbor
        return CustomCompletion{.BaseMaps = {
                                    67, // Twilight Arbor (Story)
                                    68  // Twilight Arbor (Explorable)
                                }};
    case 71: // Honor of the Waves
        return CustomCompletion{.BaseMaps = {
                                    70, // Honor of the Waves (Story)
                                    71  // Honor of the Waves (Explorable)
                                }};
    case 968: // Edge of the Mists
        return CustomCompletion{
            .BaseMaps = {968},
            .PoiExclusions = {
                // unavailable
                3204 // https://wiki.guildwars2.com/wiki/Mists_Arena#Mists_Arena_Spectator_Rally_Point
            }};
    case 1147: // Forsaken Thicket
        return CustomCompletion{.BaseMaps = {
                                    1062, // Spirit Vale
                                    1149, // Salvation Pass
                                    1156  // Stronghold of the Faithful
                                }};
    case 1413: // The Twisted Marionette (Public)
        return CustomCompletion{.BaseMaps = {1413},
                                .PoiExclusions = {
                                    // duplicates from Lornar's Pass
                                    283, // https://wiki.guildwars2.com/wiki/Winterthaw_Snowfield#Winterthaw_Waypoint
                                    282  // https://wiki.guildwars2.com/wiki/False_River_Valley#False_River_Waypoint
                                }};
    case 1477: // The Tower of Nightmares (Private Squad)
        return CustomCompletion{.BaseMaps = {1477},
                                .PoiExclusions = {
                                    // duplicate from The Tower of Nightmares (Public)
                                    1839 // https://wiki.guildwars2.com/wiki/First_Level#Tower_of_Nightmares_Waypoint
                                }};
    case 943: // The Tower of Nightmares (Public)
        return CustomCompletion{.BaseMaps = {943},
                                .PoiExclusions = {
                                    // duplicate from The Tower of Nightmares (Private Squad)
                                    3457 // https://wiki.guildwars2.com/wiki/First_Level#Tower_of_Nightmares_Waypoint
                                }};
    default:
        return std::nullopt;
    }
}

std::optional<uint32_t> TWC::Workarounds::GetMapIdReplacement(uint32_t id)
{
    switch (id)
    {
        /* clang-format off */

        // Story Mode dungeons should use Exploration Mode maps' completion
        case   33: return   36; // Ascalonian Catacombs
        case   63: return   64; // Sorrow's Embrace
        case   68: return   67; // Twilight Arbor
        case   66: return   69; // Citadel of Flame
        case   70: return   71; // Honor of the Waves
        case   75: return   76; // Caudecus's Manor
        case   81: return   82; // Crucible of Eternity

        // Verdant Brink's duplicate should use regular map's completion
        case 1042: return 1052;

        // Eye of the North accessible from Hall of Monuments Portal Stone should use
        // Icebrood's Saga Eye of the North's completion 
        case  807: return 1370;

        // The Twisted Marionette (Private Squad) should use
        // The Twisted Marionette (Public)'s completion
        case 1414: return 1413;
        
        // All of the Mount Balrior copies should use
        // Mount Balriors' completion
        case 1566: return 1564; // Map label mouse hover
        case 1562: return 1564; // Convergence: Mount Balrior (Private Squad)
        case 1571: return 1564; // Convergence: Mount Balrior (Public)
        case 1572: return 1564; // Balrior Peak: Mount Balrior

        /* clang-format on */

    default:
        return std::nullopt;
    }
}

bool TWC::Workarounds::ShouldSkipContentValidation(uint32_t id)
{
    switch (id)
    {
    // Exploration Mode Dungeon Maps are fine despite using Story Mode Map components
    case 64:
    case 69:
    case 76:
    case 82:

    // Memory of Old Lion's Arch is fine despite using Lion's Arch components
    case 1483:

    // Icebrood's Sage Eye of the North is fine despite using
    // Eye of the North accessible from Hall of Monuments Portal Stone components
    case 1370:

    // Mount Balrior is fine despite using
    // Convergence: Mount Balrior (Private Squad) components
    case 1564:
        return true;
    default:
        return false;
    }
}
