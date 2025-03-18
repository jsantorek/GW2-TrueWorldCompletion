#include "caching/CharacterCache.hpp"
#include "Constants.hpp"
#include "Logging.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

TWC::CharacterCache::~CharacterCache()
{
    try
    {
        auto json = nlohmann::json();
        json["Available"] = available_;
        json["Completion"] = completion_;
        json["Version"] = TWC::CompletionCacheVersion;
        if (!std::filesystem::exists(filepath_.parent_path()))
        {
            std::filesystem::create_directories(filepath_.parent_path());
        }
        std::ofstream(filepath_) << json;
        LOG_INFO() << " preserved " << completion_.size() << " cache entries";
    }
    catch (const std::exception &e)
    {
        LOG_FAST(WARNING, e.what());
        LOG_FAST(INFO, "Unexpected error when persisting cache");
    }
}

TWC::CharacterCache *TWC::CharacterCache::Load(std::filesystem::path filepath)
{
    auto cache = new TWC::CharacterCache();
    if (std::filesystem::exists(filepath))
    {
        try
        {
            auto json = nlohmann::json::object();
            json = nlohmann::json::parse(std::ifstream(filepath), nullptr, false);
            if (auto ver = json["Version"].get<decltype(TWC::CompletionCacheVersion)>();
                ver != TWC::CompletionCacheVersion)
            {
                LOG_INFO() << " encountered incompatible cache version, creating fresh one";
            }
            else
            {
                cache->available_ = json["Available"].get<decltype(CharacterCache::available_)>();
                cache->completion_ = json["Completion"].get<decltype(CharacterCache::completion_)>();
                LOG_INFO() << " concluded successfully with " << cache->completion_.size() << " cache entries";
            }
        }
        catch (const std::exception &e)
        {
            LOG_FAST(WARNING, e.what());
            LOG_FAST(INFO, "Unexpected error when parsing cache, creating fresh one");
        }
    }
    cache->filepath_ = std::move(filepath);
    return cache;
}
