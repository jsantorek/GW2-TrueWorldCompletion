#include "Completion/Cache.hpp"
#include "Completion/Store.hpp"
#include "Completion/Test.hpp"
#include "Completion/Value.hpp"
#include "Constant/Cache.hpp"
#include "Content/Cache.hpp"
#include "Filter/CompletionReward.hpp"
#include "Filter/Expansion.hpp"
#include "Filter/Interface.hpp"
#include "Filter/UserExclusions.hpp"
#include "Game/MissionContext.h"
#include "Logging.hpp"
#include "Model/Expansion.hpp"
#include <Game/PropContext.h>
#include <codecvt>
#include <flags/flags.hpp>
#include <fstream>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <type_traits>
#include <utility>
#include <vcruntime_typeinfo.h>
namespace
{
template <std::size_t... I> auto MakeExpansionFilters(std::index_sequence<I...>)
{
    return std::array<std::unique_ptr<TWC::FilterInterface>, sizeof...(I)>{
        std::make_unique<TWC::FilterExpansion<static_cast<TWC::Expansion>(I)>>()...};
}
} // namespace
void TWC::CompletionCache::Refresh()
{
    auto log = LOG_SCOPE(TRACE);
    std::lock_guard<std::mutex> lock(Mutex);
    TWC::CompletionTest test;
    if (!test.IsAvailable())
    {
        LOG(TRACE, "No character found");
        return;
    }
    auto ctx = GW2RE::CPropContext::Get().GetMissionCtx();
    auto store = LocalizedCompletionStore<uint32_t>{.MapId = ctx->CurrentMapID};
    const auto worldFilter = std::make_unique<FilterUserExclusions>(); // TODO
    const auto rewardFilter = FilterCompletionReward{};
    const auto expansionFilters =
        MakeExpansionFilters(std::make_index_sequence<magic_enum::enum_count<TWC::Expansion>()>{});
    G::Cache::Content->Apply([&](const auto &id) {
        if (!test(id))
            return;
        if ((*worldFilter)(id))
            ++store.World;
        if (rewardFilter(id))
            ++store.Reward;
        for (const auto expansion : magic_enum::enum_values<TWC::Expansion>())
        {
            if ((*expansionFilters[static_cast<size_t>(expansion)])(id))
                ++store.Expansions[static_cast<size_t>(expansion)];
        }
    });

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
    const auto characterName = convert.to_bytes(ctx->Character.Name);
    LOG(TRACE, "{} cached while on Map#{}", characterName, store.MapId);
    Completed.insert_or_assign(characterName, std::move(store));
}

TWC::CompletionCache::~CompletionCache()
{
    Persist();
}

TWC::CompletionCache::CompletionCache() : Available(), Hash(), Completed()
{
    Load();
}
template <typename T> struct std::hash<std::set<T>>
{
    std::size_t operator()(const std::set<T> &s) const
    {
        std::size_t h = 0;
        for (const auto &elem : s)
            h ^= std::hash<T>{}(elem) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};
void TWC::CompletionCache::Update(TWC::Options::ContentExclusion excl, Options::CompletionMode wrld)
{
    std::lock_guard<std::mutex> lock(Mutex);
    const auto worldFilter = std::make_unique<FilterUserExclusions>(); // TODO
    const auto rewardFilter = FilterCompletionReward{};
    const auto expansionFilters =
        MakeExpansionFilters(std::make_index_sequence<magic_enum::enum_count<TWC::Expansion>()>{});
    G::Cache::Content->Apply([&](const auto &id) {
        if ((*worldFilter)(id))
            ++Available.World;
        if (rewardFilter(id))
            ++Available.Reward;
        for (const auto expansion : magic_enum::enum_values<TWC::Expansion>())
        {
            if ((*expansionFilters[static_cast<size_t>(expansion)])(id))
                ++Available.Expansions[static_cast<size_t>(expansion)];
        }
    });
    auto h1 = std::hash<std::set<uint32_t>>{}(excl.Landmarks);
    auto h2 = std::hash<std::set<uint32_t>>{}(excl.Tasks);
    auto h3 = std::hash<std::set<std::string>>{}(excl.SkillChallanges);
    auto h4 = std::hash<std::underlying_type<TWC::Options::CompletionMode>::type>{}(wrld);
    auto hash = h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    if (hash == Hash)
        return;
    Completed.clear();
    Hash = hash;
}

std::optional<TWC::LocalizedCompletionStore<TWC::CompletionValue>> TWC::CompletionCache::GetCompletion(
    std::wstring_view characterName)
{
    std::lock_guard<std::mutex> lock(Mutex);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
    if (auto it = Completed.find(convert.to_bytes(characterName.data())); it != Completed.end())
    {
        const auto &completed = it->second;
        auto store = TWC::LocalizedCompletionStore<TWC::CompletionValue>{.MapId = it->second.MapId};
        store.World.Completed = completed.World;
        store.World.Available = Available.World;
        store.Reward.Completed = completed.Reward;
        store.Reward.Available = Available.Reward;
        for (const auto expansion : magic_enum::enum_values<TWC::Expansion>())
        {
            store.Expansions[static_cast<size_t>(expansion)].Completed =
                completed.Expansions[static_cast<size_t>(expansion)];
            store.Expansions[static_cast<size_t>(expansion)].Available =
                Available.Expansions[static_cast<size_t>(expansion)];
        }
        return store;
    }
    return std::nullopt;
}

void TWC::CompletionCache::Load()
{
    std::lock_guard<std::mutex> lock(Mutex);
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / CacheContstants::Filename;
    if (!std::filesystem::exists(filepath))
    {
        LOG(INFO, "File not found, fresh one will be created");
        return;
    }
    try
    {
        auto json = nlohmann::json::object();
        json = nlohmann::json::parse(std::ifstream(filepath));
        if (auto ver = json.value("Version", 0); ver != CacheContstants::Version)
        {
            LOG(WARNING, "File contains incompatible data, loading skipped");
        }
        else
        {
            json.at("Completion").get_to(Completed);
            json.at("OptionsHash").get_to(Hash);
            LOG(INFO, "Completion data of {} characters obtained", Completed.size());
        }
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when parsing cache:\n\t{}", e.what());
    }
}

void TWC::CompletionCache::Persist()
{
    std::lock_guard<std::mutex> lock(Mutex);
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / CacheContstants::Filename;
    try
    {
        auto json = nlohmann::json();
        json["Completion"] = Completed;
        json["Version"] = CacheContstants::Version;
        json["OptionsHash"] = Hash;
        if (!std::filesystem::exists(filepath.parent_path()))
        {
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream(filepath) << json.dump(4);
        LOG(INFO, "Persisted completion data of {} characters", Completed.size());
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting cache:\n\t{}", e.what());
    }
}
