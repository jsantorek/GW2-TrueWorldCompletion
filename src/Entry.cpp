
#include <Nexus.h>

#include "Globals.hpp"
#include "Hooks.hpp"

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void AddonLoad(AddonAPI *aApi);
void AddonUnload();
void OptionsRender();

extern "C" __declspec(dllexport) AddonDefinition *GetAddonDef()
{
    static AddonDefinition def{
        .Signature = 0x776E7A31,
        .APIVersion = NEXUS_API_VERSION,
        .Name = "True World Completion",
        .Version = {ADDON_VERSION_MAJOR, ADDON_VERSION_MINOR, ADDON_VERSION_PATCH, ADDON_VERSION_REVISION},
        .Author = "Vonsh.1427",
        .Description = "Seamless display within game map, includes all zones of all expansions and living worlds.",
        .Load = AddonLoad,
        .Unload = AddonUnload,
        .Flags = EAddonFlags_IsVolatile,
        // .Provider = EUpdateProvider_GitHub,
        // .UpdateLink = "https://github.com/jsantorek/GW2-TrueWorldCompletion"
    };
    return &def;
}

void AddonLoad(AddonAPI *aApi)
{
    G::APIDefs = aApi;

    // const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
    // G::Options.Parse(addonDir / "config.json");
    // ImGui::SetCurrentContext((ImGuiContext *)G::APIDefs->ImguiContext);
    // ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))G::APIDefs->ImguiMalloc,
    //                              (void (*)(void *, void *))G::APIDefs->ImguiFree);

    G::APIDefs->Renderer.Register(ERenderType_OptionsRender, OptionsRender);
    G::Hooks.Install();
    G::APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "Initialized.");
}
void AddonUnload()
{
    // const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
    // G::Options.Persist(addonDir / "config.json");
    G::Hooks.Uninstall();
    G::APIDefs->Renderer.Deregister(OptionsRender);
}

void OptionsRender()
{
}
