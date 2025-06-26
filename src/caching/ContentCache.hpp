#pragma once
#include "Logging.hpp"
#include <future>
#include <unordered_map>
#include <unordered_set>

namespace TWC
{
struct MapContent;
class ContentCache
{
  public:
    ContentCache() : initialized_(std::async(std::launch::async, &ContentCache::Initialize, this))
    {
    }

    ~ContentCache()
    {
        initialized_.get();
        content_.clear();
    }

    inline std::shared_ptr<MapContent> GetMap(uint32_t mapId)
    {
        LOG_DEBUG();
        initialized_.wait();
        if (auto it = content_.find(mapId); it != content_.end())
            return it->second;
        return nullptr;
    }

    std::unordered_set<std::shared_ptr<MapContent>> GetAll()
    {
        LOG_DEBUG();
        initialized_.wait();
        std::unordered_set<std::shared_ptr<MapContent>> uniqueContent;
        for (const auto &[_, content] : content_)
            uniqueContent.emplace(content);
        return uniqueContent;
    }

    template <class Predicate> std::unordered_set<std::shared_ptr<MapContent>> GetIf(Predicate pred)
    {
        LOG_DEBUG();
        initialized_.wait();
        std::unordered_set<std::shared_ptr<MapContent>> uniqueContent;
        for (const auto &[_, content] : content_)
            if (pred(*content))
                uniqueContent.emplace(content);
        return uniqueContent;
    }

  private:
    void Initialize();
    std::unordered_map<uint32_t, std::shared_ptr<MapContent>> content_;
    std::future<void> initialized_;
};
} // namespace TWC
namespace G::Cache
{
extern TWC::ContentCache *Content;
}
