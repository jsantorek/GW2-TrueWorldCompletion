#pragma once
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace TWC
{
struct MapContent;
class ContentCache
{
  public:
    ContentCache()
    {
        Initialize();
    }

    ~ContentCache()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        content_.clear();
    }

    inline std::shared_ptr<MapContent> GetMap(uint32_t mapId)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (auto it = content_.find(mapId); it != content_.end())
            return it->second;
        return nullptr;
    }

    std::unordered_set<std::shared_ptr<MapContent>> GetAll()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::unordered_set<std::shared_ptr<MapContent>> uniqueContent;
        for (const auto &[_, content] : content_)
            uniqueContent.emplace(content);
        return uniqueContent;
    }

    template <class Predicate> std::unordered_set<std::shared_ptr<MapContent>> GetIf(Predicate pred)
    {

        std::lock_guard<std::mutex> lock(mutex_);
        std::unordered_set<std::shared_ptr<MapContent>> uniqueContent;
        for (const auto &[_, content] : content_)
            if (pred(*content))
                uniqueContent.emplace(content);
        return uniqueContent;
    }

  private:
    void Initialize();
    std::unordered_map<uint32_t, std::shared_ptr<MapContent>> content_;
    std::mutex mutex_;
};
} // namespace TWC
namespace G::Cache
{
extern TWC::ContentCache *Content;
}
