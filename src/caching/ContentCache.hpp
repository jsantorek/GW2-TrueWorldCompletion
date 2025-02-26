#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>

namespace TWC
{
struct MapContent;
class ContentCache
{
  public:
    inline std::shared_ptr<MapContent> GetMapContent(uint32_t mapId)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (auto it = content_.find(mapId); it != content_.end())
            return it->second;
        return nullptr;
    }

    inline void Invalidate()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        content_.clear();
    }

    void EnsureValidity();

    std::vector<std::shared_ptr<MapContent>> GetAllMapsContent();

  private:
    std::unordered_map<uint32_t, std::shared_ptr<MapContent>> content_;
    std::mutex mutex_;
};
} // namespace TWC
