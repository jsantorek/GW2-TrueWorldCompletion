#pragma once

#include <map>
#include <optional>
#include <unordered_set>
#include <vector>

namespace TWC::Workarounds
{
/*
There are some maps that are not trivial to handle and
contain some inconsistencies in discoverable content
but are fine if treated as completely different map.
This function maps ids of such maps.
*/
std::optional<uint32_t> GetMapIdReplacement(uint32_t id);

/*
There are some maps with incosistencies severe enough
that substitution for another id will not fix them.
This function provides mechanism for custom completion computation.
*/
std::unordered_set<uint32_t> GetPoiExclusions(uint32_t id);

/*
There is a selection of maps that addon explicitly allows to skip, those include:
maps exclusive to gemstore, seasonal maps, retired maps that are no longer accessible
and maps exclusive to story instances.
*/
bool IsMapExplicitlyExcludable(uint32_t id);
std::map<const char *, std::vector<std::pair<uint32_t, const char *>>> GetCategorizedExplicitlyExcludableMaps();

} // namespace TWC::Workarounds
