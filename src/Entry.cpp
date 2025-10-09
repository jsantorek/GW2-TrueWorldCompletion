#include "CompletionManager.hpp"
#include "Logging.hpp"
#include "Options.hpp"
#include "StyleManager.hpp"
#include "Tools/ThreadService.h"
#include "caching/CompletionCache.hpp"
#include "caching/ContentCache.hpp"
#include "hints/HintManager.hpp"
#include "hooks/HooksManager.hpp"
#include "patches/PatchManager.hpp"
#include <Nexus.h>
#include <imgui.h>
#ifndef NDEBUG
#include "Debug.hpp"
#endif

void AddonLoad(AddonAPI *aApi);
void AddonUnload();
namespace G
{
AddonAPI *APIDefs = nullptr;
TWC::HooksManager *Hooks = nullptr;
TWC::PatchManager *Patches = nullptr;
TWC::HintManager *Hints = nullptr;
TWC::StyleManager *Style = nullptr;
TWC::CompletionManager *Completion = nullptr;
std::shared_ptr<TWC::ThreadService> Thread = nullptr;
namespace Cache
{
TWC::ContentCache *Content = nullptr;
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
        .Flags = EAddonFlags_IsVolatile,
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

    G::Hooks = new TWC::HooksManager();
    G::Style = new TWC::StyleManager();
    G::Hints = new TWC::HintManager();
    G::Completion = new TWC::CompletionManager();
    G::Cache::Content = new TWC::ContentCache();
    G::Cache::CharacterInfo = new TWC::CompletionCache();
    try
    {
        G::Thread = TWC::ThreadService::Build();
        G::Hooks->EnableCriticalHooks();
    }
    catch (const std::runtime_error &e)
    {
        LOG_FAST(CRITICAL, std::format("Critical section failure(s):\n\t{}\nAddon disabled!", e.what()).c_str());
        return;
    }
    TWC::Options::Load()->Apply();
    G::Hooks->EnableOptionalHooks();
    G::Patches = new TWC::PatchManager();
    LOG_FAST(INFO, "Hooking and patching done");
    TWC::Options::SetupConfiguration(G::APIDefs);
    G::Thread->AsyncTask(&TWC::CompletionCache::Refresh, G::Cache::CharacterInfo);

#ifndef NDEBUG
    Debug::Start();
#endif
}

void AddonUnload()
{
    // G::Thread->SyncTask(&TWC::CompletionCache::Refresh, G::Cache::CharacterInfo).get();
    TWC::Options::CleanupConfiguration(G::APIDefs);
    delete G::Style;
    delete G::Cache::CharacterInfo;
    delete G::Hooks;
    delete G::Patches;
    delete G::Hints;
    delete G::Completion;
    delete G::Cache::Content;
    G::Thread.reset();
#ifndef NDEBUG
    Debug::Stop();
#endif
}
