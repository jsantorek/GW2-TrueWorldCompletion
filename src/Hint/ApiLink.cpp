#include "Hint/ApiLink.hpp"
#include "Logging.hpp"
#include "Map/Definition.hpp"
#include <cstdint>
#include <shellapi.h>
#include <span>
#include <sstream>
#include <winuser.h>

namespace
{
// https://wiki.guildwars2.com/wiki/API:2/maps
constexpr auto UrlBase = "https://api.guildwars2.com/v2/maps?ids=";
// https://wiki-en.guildwars2.com/wiki/API:Best_practices#Making_requests
constexpr auto MapIdLimit = 200;
} // namespace

void TWC::HintApiLink::Activate()
{
    ShellExecute(nullptr, nullptr, Url.c_str(), nullptr, nullptr, SW_SHOW);
}

void TWC::HintApiLink::SetIncompleteMaps(std::span<MapDefinition> ids)
{
    if (ids.empty())
    {
        Url.clear();
        return;
    }
    std::stringstream oss;
    auto count = MapIdLimit;
    oss << UrlBase;
    for (const auto id : ids)
    {
        oss << static_cast<uint32_t>(id) << ",";
        if (!--count)
        {
            LOG(WARNING, "Exceeded maximum allowed number of map ids - linking partial results");
            break;
        }
    }
    Url = oss.str();
}

bool TWC::HintApiLink::Available()
{
    return !Url.empty();
}
