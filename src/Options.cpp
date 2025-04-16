#include "Options.hpp"
#include "Constants.hpp"
#include "HintManager.hpp"
#include "ThreadWorker.hpp"
#include "caching/CompletionCache.hpp"
#include "caching/ContentSubsetCache.hpp"
#include "model/MapClassification.hpp"
#include <Logging.hpp>
#include <Strings.hpp>
#include <fstream>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <set>

namespace TWC
{
void to_json(nlohmann::json &j, const WorldCompletionConfig &c)
{
    std::set<int32_t> ExcludedMaps(c.MapExclusions.begin(), c.MapExclusions.end());
    for (const auto &[key, inclusions] : c.MapInclusions)
    {
        for (const auto &inclusion : inclusions)
        {
            if (!inclusion.Active)
                ExcludedMaps.insert(static_cast<int32_t>(inclusion.ID));
        }
    }
    std::set<int32_t> ExcludedRegions(c.RegionExclusions.begin(), c.RegionExclusions.end());
    for (const auto &inclusion : c.RegionInclusions)
    {
        if (!inclusion.Active)
            ExcludedMaps.insert(static_cast<int32_t>(inclusion.ID));
    }
    j = nlohmann::json{{"ExcludedMaps", ExcludedMaps},
                       {"ExcludedRegions", ExcludedRegions},
                       {"IncludeMapsWithoutCompletionReward", c.IncludeMapsWithoutCompletionReward},
                       {"MapSeparation", magic_enum::enum_name(c.MapSeparation)},
                       {"ExpansionAssignment", magic_enum::enum_name(c.ExpansionAssignment)}};
}

void from_json(const nlohmann::json &j, WorldCompletionConfig &c)
{
    auto ExcludedMaps = j["ExcludedMaps"].get<std::set<int>>();
    for (auto &[key, inclusions] : c.MapInclusions)
    {
        for (auto &inclusion : inclusions)
        {
            if (auto it = ExcludedMaps.find(static_cast<int32_t>(inclusion.ID)); it != ExcludedMaps.end())
            {
                inclusion.Active = false;
                ExcludedMaps.erase(it);
            }
        }
    }
    c.MapExclusions = std::vector<uint32_t>(ExcludedMaps.begin(), ExcludedMaps.end());

    auto ExcludedRegions = j["ExcludedRegions"].get<std::set<int>>();
    for (auto &inclusion : c.RegionInclusions)
    {
        if (auto it = ExcludedRegions.find(static_cast<int32_t>(inclusion.ID)); it != ExcludedRegions.end())
        {
            inclusion.Active = false;
            ExcludedRegions.erase(it);
        }
    }
    c.RegionExclusions = std::vector<uint32_t>(ExcludedRegions.begin(), ExcludedRegions.end());
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
        G::Cache::WorldMapCompletion->Refresh(G::Options->WorldProgressWidget);
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
        G::Thread->AsyncTask(&HintManager::RequestHint, G::Hints);
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
    if (ImGui::TreeNode("WorldMap style"))
    {
        ImGui::Checkbox("Use game's default colors for map progress bars", &G::Options->DefaultColorScheme);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("WorldMap completion"))
    {
        RenderWorldCompletionConfig(G::Options->WorldProgressWidget);
        ImGui::TreePop();
    }
    static int e = G::Options->CharacterInfoWidget ? 0 : 1;
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
    if (ImGui::TreeNode("Optional region inclusions"))
    {
        for (auto &excl : cfg.RegionInclusions)
            ImGui::Checkbox(excl.Name, &excl.Active);
        ImGui::TextUnformatted("Custom");
        ImGui::SameLine();
        if (ImGui::Button(" + "))
            cfg.RegionExclusions.push_back(0);
        ImGui::SameLine();
        if (ImGui::Button(" - "))
            cfg.RegionExclusions.pop_back();
        for (std::size_t i = 0; i < cfg.RegionExclusions.size(); i++)
            ImGui::InputScalar(std::format("####{}RegionExclusions{}", cfgId, i).c_str(), ImGuiDataType_U32,
                               &cfg.RegionExclusions[i]);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Optional map inclusions"))
    {
        ImGui::Checkbox(std::format("Maps without completion reward####{}", cfgId).c_str(),
                        &cfg.IncludeMapsWithoutCompletionReward);
        for (auto &[section, inclusions] : cfg.MapInclusions)
        {
            ImGui::TextUnformatted(section);
            for (auto &excl : inclusions)
                ImGui::Checkbox(excl.Name, &excl.Active);
        }
        ImGui::TextUnformatted("Custom");
        ImGui::SameLine();
        if (ImGui::Button(" + "))
            cfg.MapExclusions.push_back(0);
        ImGui::SameLine();
        if (ImGui::Button(" - "))
            cfg.MapExclusions.pop_back();
        for (std::size_t i = 0; i < cfg.MapExclusions.size(); i++)
            ImGui::InputScalar(std::format("####{}MapExclusions{}", cfgId, i).c_str(), ImGuiDataType_U32,
                               &cfg.MapExclusions[i]);
        ImGui::TreePop();
    }
}

constexpr auto UnknownColour = 0xFFFFFFFF;
uint32_t Options::GetMapProgressBarColour(MapClassification classification) const
{
    // 0xFF318BBC default
    if (classification.Continent == MapClassification::TheMists)
        return 0xFF888B8D;
    switch (WorldProgressWidget.ExpansionAssignment == MapClassification::MapAccessibility ? classification.Expansion
                                                                                           : classification.Chronology)
    {
    case MapClassification::Core:
        return 0xFFDE2909;
    case MapClassification::HeartOfThorns:
        return 0xFF119529;
    case MapClassification::PathOfFire:
        return 0xFF661351;
    case MapClassification::EndOfDragons:
        return 0xFF32D5A0;
    case MapClassification::SecretsOfTheObscure:
        return 0xFFE6B80D;
    case MapClassification::JanthirWilds:
        return 0xFF28509F;
    default:
        break;
    }
    return UnknownColour;
}

std::unordered_set<uint32_t> WorldCompletionConfig::GetExcludedMapIds() const
{
    std::unordered_set<uint32_t> exclusions(MapExclusions.begin(), MapExclusions.end());
    for (const auto &[_, inclusions] : MapInclusions)
    {
        for (const auto &incl : inclusions)
        {
            if (incl.Active)
                continue;
            exclusions.insert(incl.ID);
        }
    }
    return exclusions;
}

std::unordered_set<uint32_t> WorldCompletionConfig::GetExcludedRegionIds() const
{
    std::unordered_set<uint32_t> exclusions(RegionExclusions.begin(), RegionExclusions.end());
    for (const auto &incl : RegionInclusions)
    {
        if (incl.Active)
            continue;
        exclusions.insert(incl.ID);
    }
    return exclusions;
}

uint32_t Options::GetWorldProgressBarColour(MapClassification classification) const
{
    if (WorldProgressWidget.MapSeparation == MapClassification::AllMapsCollectively ||
        WorldProgressWidget.MapSeparation == MapClassification::CurrentOrEarlierExpansionMaps)
    {
        return 0xFFD3B54B; // TrueWorldCompletion
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentContinentMapsOnly)
    {
        switch (classification.Continent)
        {
        case MapClassification::TheMists:
            return 0xFF888B8D;
        case MapClassification::Tyria:
            return 0xFFD3B54B;
        default:
            break;
        }
    }
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentContinentMapsOnly)
    {
        return GetMapProgressBarColour(classification);
    }
    return UnknownColour;
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
    else if (WorldProgressWidget.MapSeparation == MapClassification::CurrentContinentMapsOnly)
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
