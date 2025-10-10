#include "HintManager.hpp"
#include "Hint.hpp"
#include "Logging.hpp"
#include "Tools/ThreadService.h"
#include <memory>
#include <mutex>

TWC::HintManager::HintManager() = default;

TWC::HintManager::~HintManager() = default;

void TWC::HintManager::MarkStale()
{
    stale_ = true;
}

void TWC::Hint::Request()
{
    if (incomplete.empty())
        G::APIDefs->UI.SendAlert("This character has everything unlocked!");
    else
        this->Activate();
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
    if (!hint_)
        return;
    if (stale_)
    {
        if (TWC::ThreadService::IsThisGameThread())
            HintManager::RefreshHint();
        else
            G::Thread->SyncTask(&HintManager::RefreshHint, this).get();
        stale_ = false;
    }
    hint_->Request();
}