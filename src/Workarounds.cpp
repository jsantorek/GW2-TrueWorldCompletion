#include "Workarounds.hpp"
#include "model/Completion.hpp"

std::unordered_map<int, int> TWC::Workarounds::MapIdReplacement = {
    // Guild Halls' Hover
    {1119, 1121},
    {1141, 1124},
    {1215, 1232},
    {1417, 1462},

    // Guild Halls' Duplicates
    {1068, 1121},
    {1101, 1121},
    {1107, 1121},
    {1108, 1121},
    {1125, 1121},
    {1069, 1124},
    {1071, 1124},
    {1076, 1124},
    {1104, 1124},
    {1144, 1124},
    {1214, 1232},
    {1224, 1232},
    {1243, 1232},
    {1250, 1232},
    {1419, 1462},
    {1426, 1462},
    {1435, 1462},
    {1444, 1462},

    // private/public instance duplicates
    {1478, 1482},
    {943, 1477},
    {1326, 1327},

    // Raids
    {1186, 1188},
    {1325, 1323},
    {1566, 1564},
    {1562, 1564},

    // Fahranur
    {1268, 1307},

    // Toypokalipse
    {880, 1270},

    // Crucible of Eternity
    {81, 82},

    // Verdank Brink
    {1042, 1052},
};

std::unordered_map<int, TWC::MapCompletion> TWC::Workarounds::MapCorrections = {
    // https://wiki.guildwars2.com/wiki/Mists_Arena#Mists_Arena_Spectator_Rally_Point
    {968, TWC::MapCompletion{.Waypoints = TWC::Completion{0, -1}, .Total = TWC::Completion{0, -1}}}};

std::tuple<int, std::array<int, 3>> TWC::Workarounds::ForsakenThicket = {1147, {1062, 1149, 1156}};
