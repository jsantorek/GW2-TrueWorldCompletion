#pragma once

#include "model/MapCompletion.hpp"
#include <mutex>
#include <unordered_map>
#include <vector>

namespace TWC
{
class Cache
{
  public:
    inline void UpdateMapIdMask(std::vector<bool> mask)
    {
        std::lock_guard lock(mutex_);
        mask_ = std::move(mask);
    }

    inline bool IsValidMapId(uint32_t mapId)
    {
        std::lock_guard lock(mutex_);
        return mask_[mapId];
    }

    inline void Invalidate()
    {
        std::lock_guard lock(mutex_);
        uint32_t player_ = 0;
        cache_.clear();
        mask_.clear();
    }

    void EnsureValidity(uint32_t playerId);

    inline bool TryGet(uint32_t mapId, MapCompletion *comp)
    {
        std::lock_guard lock(mutex_);
        if (mask_[mapId])
        {
            auto it = cache_.find(mapId);
            if (it != cache_.end())
            {
                *comp = it->second;
                return true;
            }
        }
        return false;
    }

    inline void Set(uint32_t mapId, MapCompletion *comp)
    {
        std::lock_guard lock(mutex_);
        if (comp->Total.Completed == comp->Total.Available)
            cache_.emplace(mapId, *comp);
    }

    inline std::unordered_map<uint32_t, MapCompletion> GetAll()
    {
        std::lock_guard lock(mutex_);
        return cache_;
    }

  private:
    std::vector<bool> mask_;
    std::unordered_map<uint32_t, MapCompletion> cache_;
    std::mutex mutex_;
    uint32_t player_;
};
} // namespace TWC