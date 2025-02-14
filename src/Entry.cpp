
#include "Constants.hpp"
#include "Globals.hpp"
#include "Hooks.hpp"
#include "Options.hpp"
#include <Nexus.h>
#include <imgui.h>

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
    default:
        break;
    }
    return TRUE;
}

void AddonLoad(AddonAPI *aApi);
void AddonUnload();

extern "C" __declspec(dllexport) AddonDefinition *GetAddonDef()
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
        /*.Provider = EUpdateProvider_GitHub,
        .UpdateLink = "https://github.com/jsantorek/GW2-TrueWorldCompletion"*/};
    return &def;
}

void OptionsRender()
{
    const char *WorldCompletionNames[TWC::WorldCompletionMode::Count];
    WorldCompletionNames[TWC::WorldCompletionMode::CombinesAllMaps] = "totality of all maps";
    WorldCompletionNames[TWC::WorldCompletionMode::CombinesAllExplorableMaps] =
        "totality of maps with completion reward";
    WorldCompletionNames[TWC::WorldCompletionMode::SeparatesContinents] = "separate for The Mists and Tyria";
    WorldCompletionNames[TWC::WorldCompletionMode::SeparatesContinentsAndExpansions] =
        "separate for The Mists and each expansion cycle";
    WorldCompletionNames[TWC::WorldCompletionMode::Chronological] =
        "separate for The Mists and Tyria but excludes expansion cycles subsequent to current map";

    ImGui::TextUnformatted("World completion is...");
    ImGui::Combo("##World completion", &G::Options.WorldCompletion, WorldCompletionNames,
                 TWC::WorldCompletionMode::Count);
    ImGui::Separator();
    if (G::Options.WorldCompletion != TWC::WorldCompletionMode::CombinesAllMaps &&
        G::Options.WorldCompletion != TWC::WorldCompletionMode::CombinesAllExplorableMaps)
    {
        ImGui::TextUnformatted("World completion progress is displayed in...");
        ImGui::Checkbox(
            G::Options.ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::TheMists].Name,
            &G::Options.ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::TheMists].Active);
        if (G::Options.WorldCompletion == TWC::WorldCompletionMode::SeparatesContinents)
        {
            ImGui::Checkbox(
                G::Options.ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::Tyria].Name,
                &G::Options.ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::Tyria].Active);
        }
        for (auto exp = TWC::ContinentsAndExpansionsEnumeration::Core;
             G::Options.WorldCompletion == TWC::WorldCompletionMode::SeparatesContinentsAndExpansions &&
             exp < TWC::ContinentsAndExpansionsEnumeration::ToBeDetermined;
             exp = TWC::ContinentsAndExpansionsEnumeration{static_cast<uint32_t>(exp) + 1})
            ImGui::Checkbox(G::Options.ContinentsAndExpansionsInclusions[exp].Name,
                            &G::Options.ContinentsAndExpansionsInclusions[exp].Active);
        ImGui::Separator();
    }

    if (ImGui::Button("Apply and Save"))
    {
        const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
        G::Options.Persist(addonDir / TWC::ConfigFilename);
        G::Cache.Invalidate();
    }
    ImGui::SameLine();
    if (ImGui::Button("Which maps are incomplete for this character?"))
    {
        std::stringstream oss;
        oss << "https://api.guildwars2.com/v2/maps?ids=";
        auto count = TWC::HttpsMaxMapIdCount;
        for (const auto &[id, comp] : G::Cache.GetAll())
        {
            if (comp.Total.Completed == comp.Total.Available)
                continue;
            oss << id << ",";
            if (!--count)
            {
                G::APIDefs->Log(ELogLevel_WARNING, ADDON_NAME,
                                "Exceeded maximum allowed number of map ids - linking partial results");
                break;
            }
        }
        if (count != TWC::HttpsMaxMapIdCount)
            ShellExecute(nullptr, 0, oss.str().c_str(), 0, 0, SW_SHOW);
        else
            G::APIDefs->UI.SendAlert("No incomplete maps found!");
    }

    ImGui::TextUnformatted("Optional map inclusion list");
    for (auto &[section, inclusions] : G::Options.MapInclusions)
    {
        ImGui::TextUnformatted(section);
        for (auto &excl : inclusions)
        {
            ImGui::Checkbox(excl.Name, &excl.Active);
        }
    }
}

void AddonLoad(AddonAPI *aApi)
{
    G::APIDefs = aApi;

    const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
    G::Options.Parse(addonDir / TWC::ConfigFilename);
    ImGui::SetCurrentContext((ImGuiContext *)G::APIDefs->ImguiContext);
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))G::APIDefs->ImguiMalloc,
                                 (void (*)(void *, void *))G::APIDefs->ImguiFree);

    G::APIDefs->Renderer.Register(ERenderType_OptionsRender, OptionsRender);
    G::Hooks.Install();
}

void AddonUnload()
{
    const auto addonDir = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME));
    G::Options.Persist(addonDir / TWC::ConfigFilename);
    G::Hooks.Uninstall();
    G::APIDefs->Renderer.Deregister(OptionsRender);
}
