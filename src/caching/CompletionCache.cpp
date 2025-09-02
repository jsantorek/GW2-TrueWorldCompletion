#include "caching/CompletionCache.hpp"
#include "Constants.hpp"
#include "Logging.hpp"
#include "model/CachedCompletion.hpp"
#include "util/Serialization.hpp"
#include <flags/flags.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <type_traits>
#include <vcruntime_typeinfo.h>

TWC::CompletionCache::~CompletionCache()
{
    Persist();
}

TWC::CompletionCache::CompletionCache() : completion_(), hash_()
{
    Load();
}
namespace std
{
template <typename T> struct hash<std::set<T>>
{
    std::size_t operator()(const std::set<T> &s) const
    {
        std::size_t h = 0;
        for (const auto &elem : s)
            h ^= std::hash<T>{}(elem) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};
} // namespace std
void TWC::CompletionCache::Update(TWC::Options::ContentExclusion excl, Options::CompletionMode wrld)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto h1 = std::hash<std::set<uint32_t>>{}(excl.Landmarks);
    auto h2 = std::hash<std::set<uint32_t>>{}(excl.Tasks);
    auto h3 = std::hash<std::set<std::string>>{}(excl.SkillChallanges);
    auto h4 = std::hash<std::underlying_type<TWC::Options::CompletionMode>::type>{}(wrld);
    auto hash = h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    if (hash == hash_)
        return;
    completion_.clear();
    hash_ = hash;
}

std::optional<TWC::Completion> TWC::CompletionCache::GetCompletion(const std::string &characterId, uint32_t mapId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (auto it = completion_.find(characterId); it != completion_.end())
    {
        if (mapId == it->second.MapId)
            return it->second.Progress;
        else
            LOG(WARNING, "Requested cached completion of a character [{}] on MapId={}, but only MapId={} was found.",
                characterId, mapId, it->second.MapId);
    }
    return std::nullopt;
}

void TWC::CompletionCache::Load()
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
        json = nlohmann::json::parse(std::ifstream(filepath));
        if (auto ver = json.value("Version", 0); ver != CacheVersion)
        {
            LOG(WARNING, "File contains incompatible data, loading skipped");
        }
        else
        {
            json.at("Completion").get_to(completion_);
            json.at("OptionsHash").get_to(hash_);
            LOG(INFO, "Completion data of {} characters obtained", completion_.size());
        }
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when parsing cache:\n\t{}", e.what());
    }
}

void TWC::CompletionCache::Persist()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / CacheFilename;
    try
    {
        auto json = nlohmann::json();
        json["Completion"] = completion_;
        json["Version"] = CacheVersion;
        json["OptionsHash"] = hash_;
        if (!std::filesystem::exists(filepath.parent_path()))
        {
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream(filepath) << json.dump(4);
        LOG(INFO, "Persisted completion data of {} characters", completion_.size());
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting cache:\n\t{}", e.what());
    }
}
