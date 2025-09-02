#pragma once

#include "Options.hpp"
#include "model/CachedCompletion.hpp"
#include <mutex>
#include <optional>
#include <unordered_map>

namespace TWC
{
class CompletionCache
{
  public:
    CompletionCache();
    ~CompletionCache();
    std::optional<Completion> GetCompletion(const std::string &characterId, uint32_t mapId);
    void Refresh();
    void Update(Options::ContentExclusion excl, Options::CompletionMode wrld);

  private:
    void Load();
    void Persist();
    std::unordered_map<std::string, CachedCompletion> completion_;
    std::size_t hash_;
    std::mutex mutex_;
};
} // namespace TWC

namespace G::Cache
{
extern TWC::CompletionCache *CharacterInfo;
} // namespace G::Cache
