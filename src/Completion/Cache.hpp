#pragma once

#include "Completion/Store.hpp"
#include "Completion/Value.hpp"
#include "Options.hpp"
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
    std::optional<LocalizedCompletionStore<TWC::CompletionValue>> GetCompletion(std::wstring_view characterName);
    void Refresh();
    void Update(Options::ContentExclusion excl, Options::CompletionMode wrld);

  private:
    void Load();
    void Persist();

    std::unordered_map<std::string, LocalizedCompletionStore<uint32_t>> Completed;
    CompletionStore<uint32_t> Available;
    std::size_t Hash;
    std::mutex Mutex;
};
} // namespace TWC

namespace G::Cache
{
extern TWC::CompletionCache *CharacterInfo;
} // namespace G::Cache
