#include "Options.hpp"
#include "Constants.hpp"
#include "Logging.hpp"
#include <Globals.hpp>
#include <fstream>
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <set>

namespace TWC
{
Options::~Options()
{
    Persist();
    G::APIDefs->Renderer.Deregister(Render);
}

Options::Options(std::filesystem::path filepath) : _filepath(std::move(filepath))
{
    G::APIDefs->Renderer.Register(ERenderType_OptionsRender, Render);
    if (!std::filesystem::exists(_filepath))
        return;
    try
    {
        auto json = nlohmann::json::object();
        json = nlohmann::json::parse(std::ifstream(_filepath), nullptr, false);
        WorldCompletion = json.value("WorldCompletion", WorldCompletion);
        const auto ExcludedMaps = json["ExcludedMaps"].get<std::set<int>>();
        for (auto &[key, inclusions] : MapInclusions)
        {
            for (auto &inclusion : inclusions)
            {
                if (ExcludedMaps.count(static_cast<int32_t>(inclusion.ID)))
                    inclusion.Active = false;
            }
        }
        const auto ExcludedContinents = json["ExcludedContinents"].get<std::set<int>>();
        for (auto &[_, inclusion] : ContinentsAndExpansionsInclusions)
        {
            if (inclusion.ID == ContinentsAndExpansionsEnumeration::Undetermined ||
                inclusion.ID == ContinentsAndExpansionsEnumeration::ToBeDetermined)
                continue;
            if (ExcludedContinents.count(static_cast<int32_t>(inclusion.ID)))
                inclusion.Active = false;
        }
    }
    catch (const std::exception &e)
    {
        LOG_FAST(WARNING, e.what());
        LOG_FAST(WARNING, "Unexpected error when parsing options, falling back to defaults");
    }
}

void Options::Persist() const
{
    try
    {
        auto json = nlohmann::json();
        json["WorldCompletion"] = WorldCompletion;
        std::set<int> ExcludedMaps;
        for (const auto &[key, inclusions] : MapInclusions)
        {
            for (const auto &inclusion : inclusions)
            {
                if (!inclusion.Active)
                    ExcludedMaps.insert(static_cast<int32_t>(inclusion.ID));
            }
        }
        json["ExcludedMaps"] = ExcludedMaps;
        std::set<int> ExcludedContinents;
        for (const auto &[_, inclusion] : ContinentsAndExpansionsInclusions)
        {
            if (inclusion.ID == ContinentsAndExpansionsEnumeration::Undetermined ||
                inclusion.ID == ContinentsAndExpansionsEnumeration::ToBeDetermined)
                continue;
            if (!inclusion.Active)
                ExcludedContinents.insert(static_cast<int32_t>(inclusion.ID));
        }
        json["ExcludedContinents"] = ExcludedContinents;
        if (!std::filesystem::exists(_filepath.parent_path()))
        {
            std::filesystem::create_directories(_filepath.parent_path());
        }
        std::ofstream(_filepath) << json;
    }
    catch (const std::exception &e)
    {
        LOG_FAST(WARNING, e.what());
        LOG_FAST(WARNING, "Unexpected error when persisting options");
    }
}

void Options::Render()
{
    constexpr std::array<const char *, 6> WorldCompletionNames = {
        "totality of all maps",
        "totality of maps with completion reward",
        "separate for The Mists and Tyria",
        "separate for The Mists and each expansion cycle",
        "separate for The Mists and Tyria but excludes expansion cycles subsequent to current map",
        "separate for each region"};
    ImGui::TextUnformatted("World completion is...");
    ImGui::Combo("##World completion", &G::Options->WorldCompletion, WorldCompletionNames.data(),
                 WorldCompletionNames.size());
    ImGui::Separator();
    if (G::Options->WorldCompletion != TWC::WorldCompletionMode::CombinesAllMaps &&
        G::Options->WorldCompletion != TWC::WorldCompletionMode::CombinesAllExplorableMaps &&
        G::Options->WorldCompletion != TWC::WorldCompletionMode::SeparatesRegions)
    {
        ImGui::TextUnformatted("World completion progress is displayed in...");
        ImGui::Checkbox(
            G::Options->ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::TheMists].Name,
            &G::Options->ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::TheMists].Active);
        if (G::Options->WorldCompletion == TWC::WorldCompletionMode::SeparatesContinents)
        {
            ImGui::Checkbox(
                G::Options->ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::Tyria].Name,
                &G::Options->ContinentsAndExpansionsInclusions[TWC::ContinentsAndExpansionsEnumeration::Tyria].Active);
        }
        for (auto exp = TWC::ContinentsAndExpansionsEnumeration::Core;
             G::Options->WorldCompletion == TWC::WorldCompletionMode::SeparatesContinentsAndExpansions &&
             exp < TWC::ContinentsAndExpansionsEnumeration::ToBeDetermined;
             exp = TWC::ContinentsAndExpansionsEnumeration{static_cast<uint32_t>(exp) + 1})
            ImGui::Checkbox(G::Options->ContinentsAndExpansionsInclusions[exp].Name,
                            &G::Options->ContinentsAndExpansionsInclusions[exp].Active);
        ImGui::Separator();
    }

    if (ImGui::Button("Apply and Save"))
    {
        G::Options->Persist();
        G::Cache::Content->Invalidate();
        G::Cache::Completion->Clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Which maps are incomplete for this character?"))
    {
        std::stringstream oss;
        oss << "https://api.guildwars2.com/v2/maps?ids=";
        auto count = TWC::HttpsMaxMapIdCount;
        for (const auto &[id, comp] : G::Cache::Completion->GetCompletionData())
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
            ShellExecute(nullptr, nullptr, oss.str().c_str(), nullptr, nullptr, SW_SHOW);
        else
            G::APIDefs->UI.SendAlert("No incomplete maps found!");
    }
    ImGui::SameLine();
    if (ImGui::Button("Report an issue"))
    {
        ShellExecute(nullptr, nullptr, "https://github.com/jsantorek/GW2-TrueWorldCompletion/issues", nullptr, nullptr,
                     SW_SHOW);
    }

    ImGui::TextUnformatted("Optional map inclusion list");
    for (auto &[section, inclusions] : G::Options->MapInclusions)
    {
        ImGui::TextUnformatted(section);
        for (auto &excl : inclusions)
        {
            ImGui::Checkbox(excl.Name, &excl.Active);
        }
    }
}

} // namespace TWC
