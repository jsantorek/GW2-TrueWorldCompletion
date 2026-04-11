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
std::unique_ptr<TWC::HooksManager> Hooks = nullptr;
std::unique_ptr<TWC::PatchManager> Patches = nullptr;
std::unique_ptr<TWC::HintManager> Hints = nullptr;
std::unique_ptr<TWC::StyleManager> Style = nullptr;
std::unique_ptr<TWC::FilterFactory> Filters = nullptr;
namespace Cache
{
std::unique_ptr<TWC::ContentCache> Content = nullptr;
std::unique_ptr<TWC::CompletionCache> Completion = nullptr;
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
        G::Hooks = std::make_unique<TWC::HooksManager>();
        G::Hooks->EnableCriticalHooks();
    }
    catch (const std::runtime_error &e)
    {
        LOG_FAST(CRITICAL, std::format("Critical section failure(s):\n\t{}\nAddon disabled!", e.what()).c_str());
        G::Hooks.reset();
    }
    G::Style = std::make_unique<TWC::StyleManager>();
    G::Hints = std::make_unique<TWC::HintManager>();
    G::Filters = std::make_unique<TWC::FilterFactory>();
    try
    {
        const auto data = TWC::DataAccessor{};
        TWC::Converter<TWC::ContentType::RenownHeart>::Populate(data);
        G::Cache::Content = std::make_unique<TWC::ContentCache>(data);
        G::Cache::Completion = std::make_unique<TWC::CompletionCache>();
        G::Patches = std::make_unique<TWC::PatchManager>(data);
    }
    catch (const std::runtime_error &e)
    {
        LOG_FAST(CRITICAL, std::format("Data reliant section failure(s):\n\t{}\nAddon disabled!", e.what()).c_str());
        G::Cache::Content.reset();
        G::Cache::Completion.reset();
        G::Patches.reset();
        G::Hooks.reset();
        return;
    }
    G::Hooks->EnableOptionalHooks();
    LOG_FAST(INFO, "Hooking and patching done");
    TWC::Options::SetupConfiguration(G::APIDefs);
    TWC::TextLocalization::Initialize();
    TWC::Options::Load()->Apply();

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
    G::Style.reset();
    G::Cache::Completion.reset();
    G::Hooks.reset();
    G::Patches.reset();
    G::Hints.reset();
    G::Filters.reset();
    G::Cache::Content.reset();
#ifndef NDEBUG
    Debug::Stop();
#endif
}
