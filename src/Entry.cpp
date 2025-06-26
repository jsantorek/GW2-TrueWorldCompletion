
#include "ContentService.hpp"
#include "HintManager.hpp"
#include "HooksManager.hpp"
#include "Logging.hpp"
#include "Options.hpp"
#include "PatchManager.hpp"
#include "ThreadWorker.hpp"
#include "caching/CompletionCache.hpp"
#include "caching/ContentCache.hpp"
#include "caching/ContentSubsetCache.hpp"
#include <Nexus.h>
#include <imgui.h>

void AddonLoad(AddonAPI *aApi);
void AddonUnload();

namespace G
{
AddonAPI *APIDefs = nullptr;
TWC::Options *Options = nullptr;
TWC::HooksManager *Hooks = nullptr;
TWC::PatchManager *Patches = nullptr;
TWC::HintManager *Hints = nullptr;
std::shared_ptr<TWC::ThreadWorker> Thread;
std::shared_ptr<TWC::ContentService> Content;
namespace Cache
{
TWC::ContentCache *Content = nullptr;
std::shared_ptr<TWC::ContentSubsetCache> WorldMapCompletion = nullptr;
std::shared_ptr<TWC::ContentSubsetCache> CharacterInfoCompletion = nullptr;
TWC::CompletionCache *CharacterInfo = nullptr;
} // namespace Cache
} // namespace G

extern "C" __declspec(dllexport) AddonDefinition *GetAddonDef();

AddonDefinition *GetAddonDef()
{
    static AddonDefinition def{
        .Signature = 0x776E7A31,
        .APIVersion = NEXUS_API_VERSION,
        .Name = "True World Completion",
        .Version = AddonVersion{ADDON_VERSION_MAJOR, ADDON_VERSION_MINOR, ADDON_VERSION_PATCH, ADDON_VERSION_REVISION},
        .Author = "Vonsh.1427",
        .Description = "Seamless display within game map, includes all zones of all expansions and living worlds.",
        .Load = AddonLoad,
        .Unload = AddonUnload,
        .Flags = static_cast<EAddonFlags>(EAddonFlags_IsVolatile | EAddonFlags_OnlyLoadDuringGameLaunchSequence),
        .Provider = EUpdateProvider_GitHub,
        .UpdateLink = "https://github.com/jsantorek/GW2-" ADDON_NAME};
    return &def;
}

void AddonLoad(AddonAPI *aApi)
{
    G::APIDefs = aApi;
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext *>(G::APIDefs->ImguiContext));
    ImGui::SetAllocatorFunctions(reinterpret_cast<void *(*)(size_t, void *)>(G::APIDefs->ImguiMalloc),
                                 reinterpret_cast<void (*)(void *, void *)>(G::APIDefs->ImguiFree));

    G::Options = new TWC::Options();
    G::Hooks = new TWC::HooksManager();
    try
    {
        G::Thread = TWC::ThreadWorker::Build();
        G::Content = TWC::ContentService::Build();
        G::Cache::Content = new TWC::ContentCache();
        G::Hooks->EnableCriticalHooks();
    }
    catch (const std::runtime_error &e)
    {
        LOG_FAST(CRITICAL, std::format("Critical section failure(s):\n\t{}\nAddon disabled!", e.what()).c_str());
        return;
    }
    G::Hooks->EnableOptionalHooks();
    G::Patches = new TWC::PatchManager();
    LOG_FAST(INFO, "Hooking and patching done");
    G::Hints = new TWC::HintManager(G::Options->MissingMapsHint);
    G::Cache::CharacterInfo = new TWC::CompletionCache();
    G::Cache::WorldMapCompletion = std::make_shared<TWC::ContentSubsetCache>(G::Options->WorldProgressWidget);
    if (auto cfg = G::Options->CharacterInfoWidget; cfg)
    {
        G::Cache::CharacterInfoCompletion = std::make_shared<TWC::ContentSubsetCache>(*cfg);
    }
    else
    {
        G::Cache::CharacterInfoCompletion = G::Cache::WorldMapCompletion;
    }
    G::Thread->AsyncTask(&TWC::CompletionCache::Update, G::Cache::CharacterInfo);
}

void AddonUnload()
{
    G::Thread->SyncTask(&TWC::CompletionCache::Update, G::Cache::CharacterInfo).get();
    delete G::Cache::CharacterInfo;
    delete G::Hooks;
    delete G::Patches;
    delete G::Hints;
    delete G::Cache::Content;
    G::Cache::WorldMapCompletion.reset();
    G::Cache::CharacterInfoCompletion.reset();
    delete G::Options;
    G::Thread.reset();
    G::Content.reset();
}
