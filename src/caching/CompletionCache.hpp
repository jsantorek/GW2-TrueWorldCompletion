#pragma once

#include "model/Completion.hpp"
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
    std::optional<Completion> GetCompletion(const std::string &characterId);
    void Update();

  private:
    void Load();
    void Persist();
    decltype(Completion::Available) available_;
    std::unordered_map<std::string, decltype(Completion::Completed)> completion_;
    std::mutex mutex_;
};
} // namespace TWC

namespace G::Cache
{
extern TWC::CompletionCache *CharacterInfo;
} // namespace G::Cache