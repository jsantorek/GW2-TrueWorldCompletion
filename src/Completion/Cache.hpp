#pragma once

#include "Completion/Store.hpp"
#include "Completion/Value.hpp"
#include "Filter/Interface.hpp"
#include <Nexus.h>
#include <filesystem>
#include <magic_enum/magic_enum_containers.hpp>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace TWC
{
class CompletionCache
{
  public:
    CompletionCache(const AddonAPI::PathsVT &);
    ~CompletionCache();
    std::optional<LocalizedCompletionStore<TWC::CompletionValue>> GetCompletion(std::string_view);
    void Refresh();
    void Update();

  private:
    std::unordered_map<std::string, LocalizedCompletionStore<uint32_t>> Completed;
    magic_enum::containers::array<Expansion, std::unique_ptr<FilterInterface>> ExpansionFilters;
    CompletionStore<uint32_t> Available;
    std::size_t Hash;
    std::mutex Mutex;
    std::filesystem::path Filepath;
};
} // namespace TWC

namespace G::Cache
{
extern std::unique_ptr<TWC::CompletionCache> Completion;
} // namespace G::Cache
