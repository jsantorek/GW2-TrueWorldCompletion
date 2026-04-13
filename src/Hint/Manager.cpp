#include "Hint/Manager.hpp"
#include "Completion/Test.hpp"
#include "Configurable/IncompleteMapHint.hpp"
#include "Content/Cache.hpp"
#include "Content/Type.hpp"
#include "Filter/Factory.hpp"
#include "Hint/ApiLink.hpp"
#include "Hint/MapRecenter.hpp"
#include "Hint/WikiLink.hpp"
#include "Logging.hpp"
#include "Map/Definition.hpp"
#include <Nexus.h>
#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

namespace
{
constexpr auto MinimumActivationDelay =
    std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::seconds(2));

ALERTS_NOTIFY SendAlert = nullptr;
} // namespace

TWC::HintManager::HintManager(const AddonAPI::UIVT &ui) : LastActivation(std::chrono::system_clock::now())
{
    SendAlert = ui.SendAlert;
}

void TWC::HintManager::MarkStale()
{
    Stale = true;
}

void TWC::HintManager::SetHintType(ConfigurableIncompleteMapHint type)
{
    auto lock = std::lock_guard(Mutex);
    switch (type)
    {
    case ConfigurableIncompleteMapHint::OneOpenedInWiki:
        Hint = std::make_unique<HintWikiLink>();
        break;
    case ConfigurableIncompleteMapHint::OneCenteredOnWorldMap:
        Hint = std::make_unique<HintMapRecenter>();
        break;
    default:
        LOG(WARNING, "Unknown hint type, defaulting to API link");
        [[fallthrough]];
    case ConfigurableIncompleteMapHint::AllOpenedInApiLink:
        Hint = std::make_unique<HintApiLink>();
        break;
    }
    Stale = true;
}

void TWC::HintManager::RequestHint()
{
    auto lock = std::lock_guard(Mutex);
    if (Stale)
    {
        CompletionTest isComplete;
        if (!isComplete.IsAvailable())
        {
            LOG(WARNING, "Unable to determine character completion in current context");
            return;
        }
        auto isIncomplete = [&, isWorld = G::Filters->MakeWorldFilter()](auto content) {
            return (*isWorld)(content) && !isComplete(content);
        };
        auto incomplete = std::vector<MapDefinition>{};
        for (const auto def : G::Cache::Content->GetMapDefinitions())
        {
            auto content = G::Cache::Content->GetMapContent(def);
            if (!content)
            {
                continue; /* TODO */
            }
            if (content->AnyOf<ContentType::Vista>(isIncomplete) ||
                content->AnyOf<ContentType::PointOfInterest>(isIncomplete) ||
                content->AnyOf<ContentType::Waypoint>(isIncomplete) ||
                content->AnyOf<ContentType::RenownHeart>(isIncomplete) ||
                content->AnyOf<ContentType::HeroChallenge>(isIncomplete))
                incomplete.push_back(def);
        }
        if (incomplete.empty())
        {
            SendAlert("This character seems to have discovered everything"); /* TODO */
        }
        LOG(INFO, "Identified {} incomplete maps", incomplete.size());
        Hint->SetIncompleteMaps(incomplete);
        Stale = false;
    }
    const auto now = std::chrono::system_clock::now();
    if (now - LastActivation <= MinimumActivationDelay)
    {
        LOG(INFO, "Hint spam prevention mechanism");
        return;
    }
    if (Hint->Available())
    {
        Hint->Activate();
        LastActivation = now;
    }
    else
        LOG(INFO, "Hint unavailable");
}