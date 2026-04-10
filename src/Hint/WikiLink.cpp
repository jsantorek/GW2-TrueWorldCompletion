#include "Hint/WikiLink.hpp"
#include "Map/Definition.hpp"
#include <format>
#include <shellapi.h>
#include <span>
#include <winuser.h>

namespace
{
constexpr auto UrlFormat = "https://wiki.guildwars2.com/index.php?title=Special:SearchByProperty/Has_map_id/{}&limit=1";
constexpr auto HttpsMaxMapIdCount = 200;
} // namespace

void TWC::HintWikiLink::Activate()
{
    if (Next == Incomplete.end())
        Next = Incomplete.begin();
    ShellExecute(nullptr, nullptr, std::format(UrlFormat, *Next).c_str(), nullptr, nullptr, SW_SHOW);
    Next++;
}

void TWC::HintWikiLink::SetIncompleteMaps(std::span<MapDefinition> ids)
{
    Incomplete = {ids.begin(), ids.end()};
    Next = Incomplete.begin();
}

bool TWC::HintWikiLink::Available()
{
    return !Incomplete.empty();
}
