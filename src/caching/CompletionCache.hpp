#pragma once
#include "model/MapCompletion.hpp"
#include <guiddef.h>
#include <mutex>
#include <unordered_map>

namespace TWC
{
class CompletionCache
{
  public:
    inline void Clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        completion_.clear();
    }

    void EnsureValidity();

    bool TryGet(uint32_t id, MapCompletion *comp);

    inline void Set(uint32_t id, MapCompletion *comp)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        completion_.emplace(id, *comp);
    }

    inline std::unordered_map<uint32_t, MapCompletion> GetCompletionData()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return completion_;
    }

  private:
    std::unordered_map<uint32_t, MapCompletion> completion_;
    std::mutex mutex_;
    GUID player_;
};
} // namespace TWC
