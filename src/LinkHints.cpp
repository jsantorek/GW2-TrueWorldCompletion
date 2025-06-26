#include "Constants.hpp"
#include "Hint.hpp"
#include "Logging.hpp"
#include <format>
#include <sstream>

void TWC::LinkAPIHint::Activate()
{
    std::stringstream oss;
    auto count = TWC::HttpsMaxMapIdCount;
    oss << "https://api.guildwars2.com/v2/maps?ids=";
    for (const auto map : incomplete)
    {
        oss << map->ID << ",";
        if (!--count)
        {
            LOG(WARNING, "Exceeded maximum allowed number of map ids - linking partial results");
            break;
        }
    }
    ShellExecute(nullptr, nullptr, oss.str().c_str(), nullptr, nullptr, SW_SHOW);
}

void TWC::LinkWikiHint::Activate()
{
    if (incomplete.empty())
        return;
    if (last == incomplete.end())
        last = incomplete.begin();
    ShellExecute(
        nullptr, nullptr,
        std::format("https://wiki.guildwars2.com/index.php?title=Special:SearchByProperty/Has_map_id/{}&limit=1",
                    (*last)->ID)
            .c_str(),
        nullptr, nullptr, SW_SHOW);

    last++;
}