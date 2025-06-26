#include "HintManager.hpp"
#include "Common.h"
#include "ContentAnalysis.hpp"
#include "Hint.hpp"
#include "Logging.hpp"
#include "Map.h"
#include "ThreadWorker.hpp"
#include "caching/ContentSubsetCache.hpp"
#include <memory>
#include <mutex>

TWC::HintManager::HintManager(Options::MissingMapsHintMode type)
{
    SetHintType(type);
}

TWC::HintManager::~HintManager() = default;

void TWC::HintManager::MarkStale()
{
    stale_ = true;
}

void TWC::HintManager::SetHintType(Options::MissingMapsHintMode type)
{
    auto lock = std::lock_guard(mutex_);
    switch (type)
    {
    case Options::OneIncompleteZoneOpenedInWiki:
        hint_ = std::make_unique<LinkWikiHint>();
        break;
    case Options::OneIncompleteZoneCenteredOnWorldMap:
        hint_ = std::make_unique<MapRecenterHint>();
        break;
    case Options::AllIncompleteZonesShownInMapCinematic:
        hint_ = std::make_unique<MapCinematicHint>();
        break;
    default:
        LOG(WARNING, "Unknown hint type, defaulting to API link");
        [[fallthrough]];
    case Options::AllIncompleteZonesOpenedInApiLink:
        hint_ = std::make_unique<LinkAPIHint>();
        break;
    }
    stale_ = true;
}

void TWC::HintManager::RequestHint()
{
    auto lock = std::lock_guard(mutex_);
    if (stale_)
    {
        G::Thread->SyncTask(&HintManager::RefreshHint, this).get();
        stale_ = false;
    }
    hint_->Activate();
}

void TWC::HintManager::RefreshHint()
{
    auto player = GW2RE::Core::GetPropContext().GetCharCliCtx().GetControlledPlayer();
    if (player.null())
    {
        hint_->Update({});
        return;
    }
    auto poiCtx = GW2RE::Core::GetPropContext().GetPoiCtx().ptr();
    auto smgr = reinterpret_cast<GW2RE::HeroChallengeMgr_t *>(player.GetHeroChallengeMgr());
    auto tmgr = reinterpret_cast<GW2RE::TaskMgr_t *>(player.GetTaskMgr());
    auto pmgr = reinterpret_cast<GW2RE::ProgressMgr_t *>(player.GetProgressDataMgr());
    std::vector<const GW2RE::MapDef_t *> incomplete;
    for (auto map : G::Cache::WorldMapCompletion->GetMaps())
    {
        if (ContentAnalysis::IsComplete(map.get(), smgr, tmgr, pmgr, poiCtx))
            continue;
        incomplete.emplace_back(map->Def);
    }
    LOG(TRACE, "Found {} incomplete maps", incomplete.size());
    hint_->Update(std::move(incomplete));
}
