#include "Options.hpp"
#include "CompletionManager.hpp"
#include "Constants.hpp"
#include "caching/CompletionCache.hpp"
#include "caching/ContentCache.hpp"
#include "hints/HintManager.hpp"
#include "options/ColoursConfigurator.hpp"
#include "options/ExclusionsConfigurator.hpp"
#include "util/Serialization.hpp"
#include <ContentAnalysis.hpp>
#include <Logging.hpp>
#include <Strings.hpp>
#include <StyleManager.hpp>
#include <format>
#include <fstream>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <utility>

void TWC::Options::Apply() const
{
    G::Hints->SetHintType(MissingMapsHint);
    G::Cache::Content->Update(Exclusion.Tasks, Exclusion.Landmarks, Exclusion.SkillChallanges);
    G::Cache::CharacterInfo->Update(Exclusion, CharacterSelection);
    G::Style->Update(Colours, ExpansionAssignment, WorldMap);
    G::Completion->Update(WorldMap, CharacterSelection, ExpansionAssignment);
}

void TWC::Options::Save() const
{
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / ConfigFilename;
    try
    {
        if (!std::filesystem::exists(filepath.parent_path()))
            std::filesystem::create_directories(filepath.parent_path());
        auto json = nlohmann::json();
        json["Version"] = ConfigVersion;
        json["Colours"] = Colours;
        json["Exclusions"] = Exclusion;
        json["ExpansionAssignment"] = ExpansionAssignment;
        json["WorldMap"] = WorldMap;
        if (WorldMap != CharacterSelection)
            json["CharacterSelection"] = CharacterSelection;
        json["MissingMapsHint"] = MissingMapsHint;
        std::ofstream(filepath) << json.dump(4);
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting options:\n\t{}", e.what());
    }
}
std::unique_ptr<TWC::Options> TWC::Options::Load()
{
    auto options = std::make_unique<TWC::Options>();
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / ConfigFilename;
    if (!std::filesystem::exists(filepath))
    {
        LOG(INFO, "File not found, fresh one with default values will be created");
        options->Save();
        return options;
    }
    auto json = nlohmann::json::object();
    json = nlohmann::json::parse(std::ifstream(filepath), nullptr, false);
    if (auto ver = json["Version"].get<int>(); ver != ConfigVersion)
    {
        LOG(WARNING, "File contains incompatible data, loading skipped");
    }
    else
    {
        json.at("MissingMapsHint").get_to(options->MissingMapsHint);
        json.at("Exclusions").get_to(options->Exclusion);
        json.at("ExpansionAssignment").get_to(options->ExpansionAssignment);
        json.at("WorldMap").get_to(options->WorldMap);
        options->CharacterSelection = json.value("CharacterSelection", options->WorldMap);
        json.at("Colours").get_to(options->Colours);
    }
    return options;
}
namespace Config
{
std::unique_ptr<TWC::ExclusionsConfigurator> Exclusions = nullptr;
std::unique_ptr<TWC::ColourConfigurator> Colours = nullptr;
} // namespace Config

std::unique_ptr<TWC::Options> Configurable = nullptr;

void TWC::Options::SetupConfiguration(AddonAPI *api)
{
    api->Renderer.Register(ERenderType_OptionsRender, RenderConfiguration);
}

void TWC::Options::CleanupConfiguration(AddonAPI *api)
{
    api->Renderer.Deregister(RenderConfiguration);
    if (Configurable)
    {
        Configurable->Save();
        Configurable.reset();
    }
}

void TWC::Options::RenderConfiguration()
{
    if (!Configurable)
        Configurable = Options::Load();

    if (ImGui::Button("Save and Apply"))
    {
        Configurable->Save();
        Configurable->Apply();
    }
    ImGui::SameLine();
    if (ImGui::Button("Get completion hint"))
    {
        G::Hints->MarkStale();
        G::Hints->RequestHint();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cache character completion"))
    {
        G::Thread->AsyncTask(&CompletionCache::Refresh, G::Cache::CharacterInfo);
    }
    ImGui::SameLine();
    if (ImGui::Button("Report an issue"))
    {
        ShellExecute(nullptr, nullptr, "https://github.com/jsantorek/GW2-TrueWorldCompletion/issues", nullptr, nullptr,
                     SW_SHOW);
    }
    ImGui::Text("World completion displayed in character selection screen is cached when");
    ImGui::BulletText("logging out of character back to character selection screen");
    ImGui::BulletText("pressing the \"Cache character completion\" button");
    ImGui::BulletText("loading or unloading the addon");
    bool expansionFromChronology = Configurable->ExpansionAssignment == ExpansionAssignmentMode::MapReleaseChronology;
    if (ImGui::Checkbox("Map is assigned to expansion during which it was released, regardless of its accessibility",
                        &expansionFromChronology))
        Configurable->ExpansionAssignment = expansionFromChronology ? ExpansionAssignmentMode::MapReleaseChronology
                                                                    : ExpansionAssignmentMode::MapAccessibility;
    if (ImGui::BeginCombo("accounted for in world map", magic_enum::enum_name(Configurable->WorldMap).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<Options::CompletionMode>())
        {
            const bool is_selected = Configurable->WorldMap == val;
            if (ImGui::Selectable(name.data(), is_selected))
                Configurable->WorldMap = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("accounted for in character selection",
                          magic_enum::enum_name(Configurable->CharacterSelection).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<Options::CompletionMode>())
        {
            const bool is_selected = Configurable->CharacterSelection == val;
            if (ImGui::Selectable(name.data(), is_selected))
                Configurable->CharacterSelection = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Text("Hints for missing/incomplete maps/content can be requested by:");
    ImGui::BulletText("clicking on World Completion star on right side of World Progress bar");
    ImGui::BulletText("with a \"Get completion hint\" button press");
    if (ImGui::BeginCombo("after hint is requested", magic_enum::enum_name(Configurable->MissingMapsHint).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<MissingMapsHintMode>())
        {
            const bool is_selected = Configurable->MissingMapsHint == val;
            if (ImGui::Selectable(name.data(), is_selected))
                Configurable->MissingMapsHint = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::CollapsingHeader("Content excluded from completion"))
    {
        ImGui::Text("The following types of content are excluded from world completion computation");
        if (!Config::Exclusions)
            Config::Exclusions = std::make_unique<ExclusionsConfigurator>(Configurable->Exclusion.Tasks,
                                                                          Configurable->Exclusion.Landmarks,
                                                                          Configurable->Exclusion.SkillChallanges);
        if (ImGui::Button("Reset all"))
            Configurable->Exclusion = ContentExclusion{};
        Config::Exclusions->DrawIgnoreButtons();
        Config::Exclusions->Render();
    }
    if (ImGui::CollapsingHeader("Progress bar colours"))
    {
        if (!Config::Colours)
            Config::Colours = std::make_unique<ColourConfigurator>(Configurable->Colours);
        if (ImGui::Button("Reset all"))
            Configurable->Colours = Options::ProgressBarColours();
        ImGui::SameLine();
        if (ImGui::Button("Use game's default colours"))
            for (auto &[_, colour] : Configurable->Colours)
                colour = Options::ProgressBarColours::Default;
        Config::Colours->Render();
    }
}
