#pragma once
#include "model/MapClassification.hpp"
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>
#include <vector>

namespace TWC
{
struct WorldCompletionConfig;
class MapContent;
class ContentSubsetCache
{
  public:
    ContentSubsetCache(const WorldCompletionConfig &config);
    void Refresh(const WorldCompletionConfig &config);
    std::vector<std::shared_ptr<MapContent>> GetMaps();

  private:
    // void Refresh(std::unordered_set<uint32_t> mapExclusions, std::unordered_set<uint32_t> regionExclusions,
    //              bool skipRewardless);
    std::function<bool(const std::shared_ptr<MapContent> &)> GetPredicate();

    std::unordered_set<std::shared_ptr<MapContent>> subset_;
    MapClassification::ExpansionAssignmentMode expansionMode_;
    MapClassification::WorldDivisionMode mapMode_;
    std::mutex mutex_;
};
} // namespace TWC
namespace G::Cache
{
extern std::shared_ptr<TWC::ContentSubsetCache> WorldMapCompletion;
extern std::shared_ptr<TWC::ContentSubsetCache> CharacterInfoCompletion;
} // namespace G::Cache
