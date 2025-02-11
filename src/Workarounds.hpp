#pragma once

#include "model/MapCompletion.hpp"
#include <array>
#include <unordered_map>

namespace TWC::Workarounds
{
/*
For various reasons, including but not limited to:
1. Duplicate entries
2. Different map id for map hover and once loaded in game
these IDs when explicitly requested in game should be substituted with another
*/
extern std::unordered_map<int, int> MapIdReplacement;
inline int GetCorrectMapId(int id)
{
    return MapIdReplacement.count(id) ? MapIdReplacement.at(id) : id;
}

/*
Maps with unavailable pois
*/
extern std::unordered_map<int, MapCompletion> MapCorrections;
inline void CorrectCompletion(int id, MapCompletion *comp)
{
    if (auto corr = MapCorrections.find(id); corr != MapCorrections.end())
        *comp += corr->second;
}

extern std::tuple<int, std::array<int, 3>> ForsakenThicket;

} // namespace TWC::Workarounds
