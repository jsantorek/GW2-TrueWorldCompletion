#include "Completion/Cache.hpp"
#include "Content/Cache.hpp"
#include "Content/Type.hpp"
#include "DataAccessor.hpp"
#include "Filter/Factory.hpp"
#include "Hint/Manager.hpp"
#include "Logging.hpp"
#include "Model/Converter.hpp"
#include "Options.hpp"
#include "Style/Manager.hpp"
#include "Text/Localization.hpp"
#include "hooks/HooksManager.hpp"
#include "patches/PatchManager.hpp"
#include <Nexus.h>
#include <cstddef>
#include <format>
#include <future>
#include <imgui.h>
#include <stdexcept>
#ifndef NDEBUG
#include "Debug.hpp"
#endif

void AddonLoadAsync(AddonAPI *aApi);
void AddonLoad(AddonAPI *aApi);
void AddonUnload();

static std::future<void> *Initialization = nullptr;
namespace G
{
AddonAPI *APIDefs = nullptr;
TWC::HooksManager *Hooks = nullptr;
TWC::PatchManager *Patches = nullptr;
TWC::HintManager *Hints = nullptr;
TWC::StyleManager *Style = nullptr;
TWC::FilterFactory *Filters = nullptr;
namespace Cache
{
TWC::ContentCache *Content = nullptr;
TWC::CompletionCache *Completion = nullptr;
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
        .Load = AddonLoadAsync,
        .Unload = AddonUnload,
        .Flags =
#ifdef _DEBUG
            EAddonFlags_None,
#else
            EAddonFlags_IsVolatile,
#endif
        .Provider = EUpdateProvider_GitHub,
        .UpdateLink = "https://github.com/jsantorek/GW2-" ADDON_NAME};
    return &def;
}

void AddonLoadAsync(AddonAPI *aApi)
{
    Initialization = new std::future<void>(std::async(std::launch::async, AddonLoad, aApi));
}

void AddonLoad(AddonAPI *aApi)
{
    G::APIDefs = aApi;
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext *>(G::APIDefs->ImguiContext));
    ImGui::SetAllocatorFunctions(reinterpret_cast<void *(*)(size_t, void *)>(G::APIDefs->ImguiMalloc),
                                 reinterpret_cast<void (*)(void *, void *)>(G::APIDefs->ImguiFree));

    try
    {
        G::Hooks = new TWC::HooksManager();
        G::Hooks->EnableCriticalHooks();
        G::Style = new TWC::StyleManager();
        G::Hints = new TWC::HintManager();
        G::Filters = new TWC::FilterFactory();
        const auto data = TWC::DataAccessor{};
        TWC::Converter<TWC::ContentType::RenownHeart>::Populate(data);
        G::Cache::Content = new TWC::ContentCache(data);
        G::Cache::Completion = new TWC::CompletionCache();
        G::Patches = new TWC::PatchManager(data);
        G::Hooks->EnableOptionalHooks();
        LOG_FAST(INFO, "Hooking and patching done");
        TWC::Options::SetupConfiguration(G::APIDefs);
        TWC::TextLocalization::Initialize();
        TWC::Options::Load()->Apply();
    }
    catch (const std::runtime_error &e)
    {
        LOG_FAST(CRITICAL, std::format("Critical section failure(s):\n\t{}\nAddon disabled!", e.what()).c_str());
        return;
    }

#ifndef NDEBUG
    Debug::Start();
#endif
}

void AddonUnload()
{
    Initialization->wait();
    delete Initialization;
    TWC::Converter<TWC::ContentType::RenownHeart>::Clear();
    TWC::Options::CleanupConfiguration(G::APIDefs);
    delete G::Style;
    delete G::Cache::Completion;
    delete G::Hooks;
    delete G::Patches;
    delete G::Hints;
    delete G::Filters;
    delete G::Cache::Content;
#ifndef NDEBUG
    Debug::Stop();
#endif
}
