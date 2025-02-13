#include "Cache.hpp"
#include "Globals.hpp"

void TWC::Cache::EnsureValidity(uint32_t playerId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (player_ == playerId && !mask_.empty())
        return;
    cache_.clear();
    player_ = playerId;
    mask_ = G::Hooks.GetMapIdMask();
}
