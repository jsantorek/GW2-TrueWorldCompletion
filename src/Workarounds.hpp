#pragma once

#include "model/MapCompletion.hpp"
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

/*
Maps with unavailable pois
*/
extern std::unordered_map<int, MapCompletion> MapCorrections;

} // namespace TWC::Workarounds
