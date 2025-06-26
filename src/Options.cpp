#include "Options.hpp"
#include "Common.h"
#include "Constants.hpp"
#include "HintManager.hpp"
#include "Types.h"
#include "caching/CompletionCache.hpp"
#include "caching/ContentCache.hpp"
#include "caching/ContentSubsetCache.hpp"
#include "model/Colour.hpp"
#include "model/MapClassification.hpp"
#include <ContentAnalysis.hpp>
#include <Logging.hpp>
#include <Strings.hpp>
#include <ThreadWorker.hpp>
#include <Workarounds.hpp>
#include <algorithm>
#include <fstream>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>

namespace TWC
{
void to_json(nlohmann::json &j, const WorldCompletionConfig &c)
{
    j = nlohmann::json{{"ExcludedMaps", c.MapExclusions},
                       {"ExcludedRegions", c.RegionExclusions},
                       {"IncludeMapsWithoutCompletionReward", c.IncludeMapsWithoutCompletionReward},
                       {"MapSeparation", magic_enum::enum_name(c.MapSeparation)},
                       {"ExpansionAssignment", magic_enum::enum_name(c.ExpansionAssignment)}};
}

void from_json(const nlohmann::json &j, WorldCompletionConfig &c)
{
    c.MapExclusions = j["ExcludedMaps"].get<std::unordered_set<uint32_t>>();
    c.RegionExclusions = j["ExcludedRegions"].get<std::unordered_set<uint32_t>>();
    j.at("IncludeMapsWithoutCompletionReward").get_to(c.IncludeMapsWithoutCompletionReward);

    c.MapSeparation =
        magic_enum::enum_cast<MapClassification::WorldDivisionMode>(j.at("MapSeparation").get<std::string>())
            .value_or(c.MapSeparation);
    c.ExpansionAssignment = magic_enum::enum_cast<MapClassification::ExpansionAssignmentMode>(
                                j.at("ExpansionAssignment").get<std::string>())
                                .value_or(c.ExpansionAssignment);
}
} // namespace TWC

using namespace TWC;

Options::~Options()
{
    Persist();
    G::APIDefs->Renderer.Deregister(Render);
}

Options::Options()
{
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / ConfigFilename;
    G::APIDefs->Renderer.Register(ERenderType_OptionsRender, Render);
    if (!std::filesystem::exists(filepath))
    {
        LOG(INFO, "File not found, fresh one will be created");
        return;
    }
    try
    {
        auto json = nlohmann::json::object();
        json = nlohmann::json::parse(std::ifstream(filepath), nullptr, false);
        if (auto ver = json["Version"].get<int>(); ver != ConfigVersion)
        {
            LOG(WARNING, "File contains incompatible data, loading skipped");
        }
        else
        {
            DefaultColorScheme = json.value("DefaultColorScheme", DefaultColorScheme);
            WorldProgressWidget = json.at("WorldProgressWidget").get<TWC::WorldCompletionConfig>();
            if (json.contains("CharacterInfoWidget"))
            {
                CharacterInfoWidget = json.at("CharacterInfoWidget").get<TWC::WorldCompletionConfig>();
            }
            MissingMapsHint = magic_enum::enum_cast<MissingMapsHintMode>(json.at("MissingMapsHint").get<std::string>())
                                  .value_or(MissingMapsHint);
        }
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when parsing options:\n\t{}", e.what());
    }
}

void Options::Persist() const
{
    auto filepath = std::filesystem::path(G::APIDefs->Paths.GetAddonDirectory(ADDON_NAME)) / ConfigFilename;
    try
    {
        if (!std::filesystem::exists(filepath.parent_path()))
        {
            std::filesystem::create_directories(filepath.parent_path());
        }
        auto json = nlohmann::json();
        json["Version"] = ConfigVersion;
        json["DefaultColorScheme"] = DefaultColorScheme;
        json["WorldProgressWidget"] = WorldProgressWidget;
        json["MissingMapsHint"] = magic_enum::enum_name(MissingMapsHint);
        if (CharacterInfoWidget)
            json["CharacterInfoWidget"] = CharacterInfoWidget.value();
        std::ofstream(filepath) << json;
    }
    catch (const std::exception &e)
    {
        LOG(WARNING, "Unexpected error when persisting options:\n\t{}", e.what());
    }
}

void Options::Render()
{
    if (ImGui::Button("Apply and Save"))
    {
        G::Options->Persist();
        G::Cache::WorldMapCompletion->QueueRefresh(G::Options->WorldProgressWidget);
        G::Hints->SetHintType(G::Options->MissingMapsHint);
        if (G::Options->CharacterInfoWidget)
        {
            G::Cache::CharacterInfoCompletion = std::make_shared<ContentSubsetCache>(*G::Options->CharacterInfoWidget);
        }
        else
        {
            G::Cache::CharacterInfoCompletion = G::Cache::WorldMapCompletion;
        }
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
        G::Thread->AsyncTask(&CompletionCache::Update, G::Cache::CharacterInfo);
    }
    ImGui::SameLine();
    if (ImGui::Button("Report an issue"))
    {
        ShellExecute(nullptr, nullptr, "https://github.com/jsantorek/GW2-TrueWorldCompletion/issues", nullptr, nullptr,
                     SW_SHOW);
    }
    if (ImGui::BeginCombo("after WorldCompletion star clicked",
                          magic_enum::enum_name(G::Options->MissingMapsHint).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<MissingMapsHintMode>())
        {
            const bool is_selected = G::Options->MissingMapsHint == val;
            if (ImGui::Selectable(name.data(), is_selected))
                G::Options->MissingMapsHint = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Checkbox("Use game's default colors for map progress bars", &G::Options->DefaultColorScheme);

    if (ImGui::TreeNode("WorldMap completion"))
    {
        RenderWorldCompletionConfig(G::Options->WorldProgressWidget);
        ImGui::TreePop();
    }
    int e = G::Options->CharacterInfoWidget ? 1 : 0;
    ImGui::TextUnformatted("CharSelection screen completion uses");
    ImGui::SameLine();
    if (ImGui::RadioButton("WorldMap settings", &e, 0))
        G::Options->CharacterInfoWidget = std::nullopt;
    ImGui::SameLine();
    if (ImGui::RadioButton("custom settings", &e, 1))
        G::Options->CharacterInfoWidget = G::Options->WorldProgressWidget;
    if (G::Options->CharacterInfoWidget && ImGui::TreeNode("CharSelection screen completion"))
    {
        RenderWorldCompletionConfig(G::Options->CharacterInfoWidget.value());
        ImGui::TreePop();
    }
}

void Options::RenderWorldCompletionConfig(WorldCompletionConfig &cfg)
{
    const auto cfgId = reinterpret_cast<uint64_t>(&cfg);
    if (ImGui::BeginCombo("accounted for in calculation", magic_enum::enum_name(cfg.MapSeparation).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<MapClassification::WorldDivisionMode>())
        {
            const bool is_selected = cfg.MapSeparation == val;
            if (ImGui::Selectable(name.data(), is_selected))
                cfg.MapSeparation = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("determines its expansion assignment", magic_enum::enum_name(cfg.ExpansionAssignment).data()))
    {
        for (auto [val, name] : magic_enum::enum_entries<MapClassification::ExpansionAssignmentMode>())
        {
            const bool is_selected = cfg.ExpansionAssignment == val;
            if (ImGui::Selectable(name.data(), is_selected))
                cfg.ExpansionAssignment = val;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Checkbox(std::format("Maps without completion reward####{}", cfgId).c_str(),
                    &cfg.IncludeMapsWithoutCompletionReward);
    if (cfg.IncludeMapsWithoutCompletionReward && ImGui::TreeNode("Included regions"))
    {
        constexpr auto Regions = std::array<std::pair<uint32_t, const char *>, 2>{
            std::pair<uint32_t, const char *>{6, "Player vs. Player"}, {7, "World vs. World"}};
        for (const auto &[id, name] : Regions)
        {
            bool isActive = !cfg.RegionExclusions.contains(id);
            if (ImGui::Checkbox(name, &isActive))
            {
                if (isActive)
                    cfg.RegionExclusions.erase(id);
                else
                    cfg.RegionExclusions.emplace(id);
            }
        }
        ImGui::TreePop();
    }
    if (cfg.IncludeMapsWithoutCompletionReward && ImGui::TreeNode("Optional map inclusions"))
    {
        static auto ExplicitInclusions = Workarounds::GetCategorizedExplicitlyExcludableMaps();
        for (auto &[cat, maps] : ExplicitInclusions)
        {
            ImGui::TextUnformatted(cat);
            for (const auto &[id, name] : maps)
            {
                bool isActive = !cfg.MapExclusions.contains(id);
                if (ImGui::Checkbox(name, &isActive))
                {
                    if (isActive)
                        cfg.MapExclusions.erase(id);
                    else
                        cfg.MapExclusions.emplace(id);
                }
            }
        }
        ImGui::TextUnformatted("Custom");
        static auto CustomInclusions = std::vector<std::pair<uint32_t, GW2RE::TextHash>>();
        for (const auto &[id, name] : CustomInclusions)
        {
            bool isActive = !cfg.MapExclusions.contains(id);
            if (ImGui::Checkbox(std::format("{} id={}", name, id).c_str(), &isActive))
            {
                if (isActive)
                    cfg.MapExclusions.erase(id);
                else
                    cfg.MapExclusions.emplace(id);
            }
        }
        static uint32_t newId = 0;
        static std::string error = "";
        if (ImGui::InputScalar("Add Map", ImGuiDataType_U32, &newId, nullptr, nullptr, nullptr,
                               ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (auto map = G::Cache::Content->GetMap(newId); !map || !map->Def->Name)
            {
                error = "Map#%u couldn't be found or has no discoverable content.";
            }
            else if (Workarounds::IsMapExplicitlyExcludable(newId))
            {
                error = "Map#%u already available for explicit exclusion.";
            }
            else if (std::any_of(CustomInclusions.begin(), CustomInclusions.end(),
                                 [id = newId](auto id_name) { return id_name.first == id; }))
            {
                error = "Map#%u already added.";
            }
            else
            {
                if (map->Def->ID != newId)
                    error = std::format("Map#{} uses content of Map#{}", newId, map->Def->ID);
                else
                    error = "";
                CustomInclusions.emplace_back(newId, map->Def->Name);
                newId = 0;
            }
        }
        if (!error.empty())
        {
            ImGui::TextColored({1, 0, 0, 1}, error.c_str(), newId);
        }
        ImGui::TreePop();
    }
}

Colour4 Options::GetMapProgressBarColour(MapClassification classification) const
{
    if (DefaultColorScheme)
        return Colours::Default;
    if (classification.Continent == MapClassification::TheMists)
        return Colours::TheMists;
    switch (WorldProgressWidget.ExpansionAssignment == MapClassification::MapAccessibility ? classification.Expansion
                                                                                           : classification.Chronology)
    {
    case MapClassification::Core:
        return Colours::Core;
    case MapClassification::HeartOfThorns:
        return Colours::HeartOfThorns;
    case MapClassification::PathOfFire:
        return Colours::PathOfFire;
    case MapClassification::EndOfDragons:
        return Colours::EndOfDragons;
    case MapClassification::SecretsOfTheObscure:
        return Colours::SecretsOfTheObscure;
    case MapClassification::JanthirWilds:
        return Colours::JanthirWilds;
    default:
        break;
    }
    return Colours::Unknown;
}

Colour4 Options::GetWorldProgressBarColour(MapClassification classification) const
{
    if (DefaultColorScheme)
        return Colours::Default;
    if (WorldProgressWidget.MapSeparation == MapClassification::AllMapsCollectively ||
        WorldProgressWidget.MapSeparation == MapClassification::CurrentOrEarlierExpansionMaps)
    {
        return Colours::Legendary;
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentContinentMapsOnly)
    {
        switch (classification.Continent)
        {
        case MapClassification::TheMists:
            return Colours::TheMists;
        case MapClassification::Tyria:
            return Colours::Default;
        default:
            break;
        }
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentExpansionMapsOnly)
    {
        return GetMapProgressBarColour(classification);
    }
    return Colours::Unknown;
}

uint32_t Options::GetWorldProgressBarTitle(MapClassification classification) const
{
    if (WorldProgressWidget.MapSeparation == MapClassification::AllMapsCollectively ||
        WorldProgressWidget.MapSeparation == MapClassification::CurrentOrEarlierExpansionMaps)
    {
        return Strings::Legendary;
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentContinentMapsOnly)
    {
        switch (classification.Continent)
        {
        case MapClassification::TheMists:
            return Strings::TheMists;
        case MapClassification::Tyria:
            return Strings::Tyria;
        default:
            break;
        }
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentExpansionMapsOnly)
    {
        if (classification.Continent == MapClassification::TheMists)
            return Strings::TheMists;
        switch (classification.Expansion)
        {
        case MapClassification::Core:
            return Strings::CentralTyria;
        case MapClassification::HeartOfThorns:
            return Strings::HeartOfThorns;
        case MapClassification::PathOfFire:
            return Strings::PathOfFire;
        case MapClassification::EndOfDragons:
            return Strings::EndOfDragons;
        case MapClassification::SecretsOfTheObscure:
            return Strings::SecretsOfTheObscure;
        case MapClassification::JanthirWilds:
            return Strings::JanthirWilds;
        default:
            break;
        }
    }
    return Strings::QuestionMarks;
}
