
#include "Constants.hpp"
#include "Globals.hpp"
#include "Hooks.hpp"
#include "Options.hpp"
#include <Nexus.h>
#include <imgui.h>

void AddonLoad(AddonAPI *aApi);
void AddonUnload();
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
        .Flags = EAddonFlags_None,
        .Provider = EUpdateProvider_GitHub,
        .UpdateLink = "https://github.com/jsantorek/GW2-" ADDON_NAME};
    return &def;
}

void AddonLoad(AddonAPI *aApi)
{
    G::APIDefs = aApi;
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext *>(G::APIDefs->ImguiContext));
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))G::APIDefs->ImguiMalloc,
                                 (void (*)(void *, void *))G::APIDefs->ImguiFree);
    G::Cache::Content = new TWC::ContentCache();
    G::Cache::Completion = new TWC::CompletionCache();
    const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
    G::Cache::Characters = TWC::CharacterCache::Load(addonDir / TWC::CacheFilename);
    G::Options = new TWC::Options(addonDir / TWC::ConfigFilename);
    try
    {
        G::Hooks.Install();
    }
    catch (const std::runtime_error &e)
    {
        G::APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, e.what());
        G::APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, "Addon remains disabled!");
    }
}

void AddonUnload()
{
    G::Cache::Delete();
    delete G::Options;
    G::Options = nullptr;
    G::Hooks.Uninstall();
    G::APIDefs = nullptr;
}
