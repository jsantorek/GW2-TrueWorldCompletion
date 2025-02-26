
#include "Constants.hpp"
#include "Globals.hpp"
#include "Hooks.hpp"
#include "Options.hpp"
#include <Nexus.h>
#include <imgui.h>

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }
    return TRUE;
}

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
    G::Options = new TWC::Options(addonDir / TWC::ConfigFilename);
    G::Hooks.Install();
}

void AddonUnload()
{
    G::Cache::Delete();
    delete G::Options;
    G::Options = nullptr;
    G::Hooks.Uninstall();
    G::APIDefs = nullptr;
}
