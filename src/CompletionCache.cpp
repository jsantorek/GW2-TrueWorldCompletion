#include "caching/CompletionCache.hpp"
#include "Constants.hpp"
#include "Logging.hpp"
#include "ThreadWorker.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace TWC;

CompletionCache::~CompletionCache()
{
    Persist();
}

CompletionCache::CompletionCache() : available_(0), completion_()
{
    Load();
}

std::optional<Completion> CompletionCache::GetCompletion(const std::string &characterId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (auto it = completion_.find(characterId); it != completion_.end())
        return Completion{it->second, available_};
    return std::nullopt;
}

void CompletionCache::Load()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / CacheFilename;
    if (!std::filesystem::exists(filepath))
    {
        LOG(INFO, "File not found, fresh one will be created");
        return;
    }
    try
    {
        auto json = nlohmann::json::object();
        json = nlohmann::json::parse(std::ifstream(filepath), nullptr, false);
        if (auto ver = json["Version"].get<decltype(CacheVersion)>(); ver != CacheVersion)
        {
            LOG(WARNING, "File contains incompatible data, loading skipped");
        }
        else
        {
            completion_ = json["Completion"].get<decltype(CompletionCache::completion_)>();
            available_ = json["Available"].get<decltype(CompletionCache::available_)>();
            LOG(INFO, "Loaded completion data of {} characters", completion_.size());
        }
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when parsing cache:\n\t{}", e.what());
    }
}

void CompletionCache::Persist()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / CacheFilename;
    try
    {
        auto json = nlohmann::json();
        json["Available"] = available_;
        json["Completion"] = completion_;
        json["Version"] = CacheVersion;
        if (!std::filesystem::exists(filepath.parent_path()))
        {
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream(filepath) << json;
        LOG(INFO, "Persisted completion data of {} characters", completion_.size());
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting cache:\n\t{}", e.what());
    }
}
