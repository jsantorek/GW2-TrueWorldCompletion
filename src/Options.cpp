#include "Options.hpp"
#include "Completion/Cache.hpp"
#include "Configurable/ExpansionAssignment.hpp"
#include "Configurable/IncompleteMapHint.hpp"
#include "Configurable/MinorPatch.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Content/Cache.hpp"
#include "Filter/Factory.hpp"
#include "Hint/Manager.hpp"
#include "Patch/Manager.hpp"
#include "Style/Manager.hpp"
#include <Logging.hpp>
#include <Nexus.h>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <shellapi.h>
#include <string_view>
#include <winuser.h>
namespace
{
constexpr auto Version = 2;
constexpr auto Filename = "config.json";
GUI_REMRENDER Deregister = nullptr;
PATHS_GETADDONDIR GetAddonDirectory = nullptr;
} // namespace
void TWC::Options::Apply() const
{
    G::Hints->SetHintType(IncompleteMapHint);
    G::Filters->Update(ExpansionAssignment, WorldCompletion);
    G::Style->Update(ColourPalette, ExpansionAssignment, WorldCompletion, TextFormat);
    G::Cache::Content->Update(ContentExclusions);
    G::Patches->Update<ConfigurableMinorPatch::DiscoveryAlerts>(
        MinorPatches.contains(ConfigurableMinorPatch::DiscoveryAlerts));
    G::Patches->Update<ConfigurableMinorPatch::LoadingScreens>(
        MinorPatches.contains(ConfigurableMinorPatch::LoadingScreens));
    G::Patches->Update<ConfigurableMinorPatch::SpiritValeMapLabelName>(
        MinorPatches.contains(ConfigurableMinorPatch::SpiritValeMapLabelName));
    G::Patches->Update<ConfigurableMinorPatch::WorldMapProgress>(
        MinorPatches.contains(ConfigurableMinorPatch::WorldMapProgress));
    G::Cache::Completion->Update();
}

void TWC::Options::Save() const
{
    auto filepath = std::filesystem::path(GetAddonDirectory(ADDON_NAME)) / Filename;
    try
    {
        if (!std::filesystem::exists(filepath.parent_path()))
            std::filesystem::create_directories(filepath.parent_path());
        auto json = nlohmann::json();
        json["Version"] = Version;
        json["MinorPatches"] = MinorPatches;
        json["IncompleteMapHint"] = IncompleteMapHint;
        json["ContentExclusions"] = ContentExclusions;
        json["ColourPalette"] = ColourPalette;
        json["WorldCompletion"] = WorldCompletion;
        json["ExpansionAssignment"] = ExpansionAssignment;
        json["TextFormat"] = TextFormat;
        std::ofstream(filepath) << json.dump(4);
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting options:\n\t{}", e.what());
    }
}

std::unique_ptr<TWC::Options> TWC::Options::Load()
{
    auto filepath = std::filesystem::path(GetAddonDirectory(ADDON_NAME)) / Filename;
    auto options = std::make_unique<TWC::Options>();
    if (!std::filesystem::exists(filepath))
    {
        LOG(INFO, "File not found, fresh one with default values will be created");
        options->Save();
        return options;
    }
    try
    {
        auto json = nlohmann::json::object();
        json = nlohmann::json::parse(std::ifstream(filepath));
        if (auto ver = json.value("Version", 0); ver != Version)
        {
            LOG(WARNING, "File contains incompatible data, loading skipped");
        }
        else
        {
            json.at("MinorPatches").get_to(options->MinorPatches);
            json.at("IncompleteMapHint").get_to(options->IncompleteMapHint);
            json.at("ContentExclusions").get_to(options->ContentExclusions);
            json.at("ColourPalette").get_to(options->ColourPalette);
            json.at("WorldCompletion").get_to(options->WorldCompletion);
            json.at("ExpansionAssignment").get_to(options->ExpansionAssignment);
            json.at("TextFormat").get_to(options->TextFormat);
        }
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when parsing options:\n\t{}", e.what());
    }
    return options;
}

static std::unique_ptr<TWC::Options> Configurable = nullptr;

void TWC::Options::SetupConfiguration(const AddonAPI::RendererVT &renderer, const AddonAPI::PathsVT &paths)
{
    renderer.Register(ERenderType_OptionsRender, RenderConfiguration);
    Deregister = renderer.Deregister;
    GetAddonDirectory = paths.GetAddonDirectory;
}

void TWC::Options::CleanupConfiguration()
{
    Deregister(RenderConfiguration);
    if (Configurable)
    {
        Configurable->Save();
        Configurable.reset();
    }
}
magic_enum::containers::array<TWC::ConfigurableExpansionAssignment, std::string_view> TWC::Options::
    MakeAssignmentNames()
{
    auto names = magic_enum::containers::array<ConfigurableExpansionAssignment, std::string_view>{};
    names.fill(""); /* TODO */
    names[ConfigurableExpansionAssignment::AccessabilityBased] =
        "Maps accessible to core accounts are not assigned to any expansion";
    names[ConfigurableExpansionAssignment::ChronologyBased] =
        "Maps are assigned to an expansion if they were released during its era";
    return names;
}

magic_enum::containers::array<TWC::ConfigurableIncompleteMapHint, std::string_view> TWC::Options::MakeHintNames()
{
    auto names = magic_enum::containers::array<ConfigurableIncompleteMapHint, std::string_view>{};
    names.fill(""); /* TODO */
    names[ConfigurableIncompleteMapHint::AllOpenedInApiLink] = "All incomplete maps are opened through GW2 API link";
    names[ConfigurableIncompleteMapHint::OneOpenedInWiki] = "One incomplete map is searched for by id in GW2 Wiki";
    names[ConfigurableIncompleteMapHint::OneCenteredOnWorldMap] = "In-game map is centered on one incomplete map";
    return names;
}
magic_enum::containers::array<TWC::ConfigurableWorldCompletion, std::string_view> TWC::Options::MakeCompletionNames()
{
    auto names = magic_enum::containers::array<ConfigurableWorldCompletion, std::string_view>{};
    names.fill(""); /* TODO */
    names[ConfigurableWorldCompletion::AllMapsCollectively] = "Legendary: All content accounted for together";
    names[ConfigurableWorldCompletion::AllMapsWithCompletionReward] =
        "Reward: All content required for map completion rewards";
    names[ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps] =
        "All content from maps in character's current or earlier expansion";
    names[ConfigurableWorldCompletion::CurrentContinentMapsOnly] =
        "Continent: All content from maps in character's current continent";
    names[ConfigurableWorldCompletion::CurrentExpansionMapsOnly] =
        "Expansion: All content from maps in character's current expansion";
    return names;
}

void TWC::Options::RenderGeneral()
{
    if (ImGui::Button("Get completion hint"))
    {
        G::Hints->MarkStale();
        G::Hints->RequestHint();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Hints for missing/incomplete maps/content can be requested by:"
                               "\n\t- clicking on World Completion star on right side of World Progress bar"
                               "\n\t- pressing this button");

        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cache character completion"))
        G::Cache::Completion->Refresh();
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("World completion displayed in character selection screen is cached when"
                               "\n\t- logging out of character back to character selection screen"
                               "\n\t- shutting down game"
                               "\n\t- pressing this button");
        ImGui::EndTooltip();
    }

    static const auto assignmentNames = MakeAssignmentNames();
    if (ImGui::BeginCombo("Expansion assignment mode", assignmentNames[Configurable->ExpansionAssignment].data()))
    {
        for (auto val : magic_enum::enum_values<ConfigurableExpansionAssignment>())
        {
            const bool is_selected = Configurable->ExpansionAssignment == val;
            if (ImGui::Selectable(assignmentNames[val].data(), is_selected))
                Configurable->ExpansionAssignment = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(
            "Some maps (eg Guild Halls or Homesteads) can be assigned an expansion based on"
            "\neither their accessibility (eg core accounts being able to join any guild hall or homestead)"
            "\nor their chronological release order (eg Thousand Seas Pavilion being released during End of Dragons)");
        ImGui::EndTooltip();
    }

    static const auto completionNames = MakeCompletionNames();
    if (ImGui::BeginCombo("World completion mode", completionNames[Configurable->WorldCompletion].data()))
    {
        for (auto val : magic_enum::enum_values<ConfigurableWorldCompletion>())
        {
            const bool is_selected = Configurable->WorldCompletion == val;
            if (ImGui::Selectable(completionNames[val].data(), is_selected))
                Configurable->WorldCompletion = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(
            "There are several ways in which content can be accounted for when computing \"World Completion\"."
            "\nThis selection impacts character selection value and colour coding depending on current character "
            "location");
        ImGui::EndTooltip();
    }

    static const auto hintNames = MakeHintNames();
    if (ImGui::BeginCombo("Incomplete map hint type", hintNames[Configurable->IncompleteMapHint].data()))
    {
        for (auto val : magic_enum::enum_values<ConfigurableIncompleteMapHint>())
        {
            const bool is_selected = Configurable->IncompleteMapHint == val;
            if (ImGui::Selectable(hintNames[val].data(), is_selected))
                Configurable->IncompleteMapHint = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Hint type issued upon request");
        ImGui::EndTooltip();
    }
}
void TWC::Options::RenderUtilityButtons()
{
    if (ImGui::Button("Save and apply"))
    {
        Configurable->Save();
        Configurable->Apply();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset defaults"))
        Configurable = std::make_unique<Options>();
    ImGui::SameLine();
    if (ImGui::Button("Reload from file"))
        Configurable = Options::Load();
    ImGui::SameLine();
    if (ImGui::Button("Report an issue (GitHub)"))
    {
        ShellExecute(nullptr, nullptr, "https://github.com/jsantorek/GW2-TrueWorldCompletion/issues", nullptr, nullptr,
                     SW_SHOW);
    }
    ImGui::SameLine();
    if (ImGui::Button("Get help (Discord)"))
    {
        ShellExecute(nullptr, nullptr, "https://discord.com/channels/410828272679518241/1336830031053324308", nullptr,
                     nullptr, SW_SHOW);
    }
}

void TWC::Options::RenderConfiguration()
{
    if (!Configurable)
        Configurable = Options::Load();
    RenderUtilityButtons();
    if (ImGui::BeginTabBar("OptionsCategories", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("General"))
        {
            Configurable->RenderGeneral();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Minor Patches"))
        {
            Configurable->MinorPatches.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Text Formatting"))
        {
            Configurable->TextFormat.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Colour Palette"))
        {
            Configurable->ColourPalette.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Content Exclusions"))
        {
            Configurable->ContentExclusions.Render();
            ImGui::EndTabItem();
        }
    }
}
