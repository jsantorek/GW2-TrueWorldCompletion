#include "ContentExclusions.hpp"
#include "Configurable/ExclusionReason.hpp"
#include "Content/Cache.hpp"
#include "Content/Container.hpp"
#include "Content/Features.hpp"
#include "Content/Record.hpp"
#include "Content/Type.hpp"
#include "Filter/ExcludedFeatures.hpp"
#include "Filter/Interface.hpp"
#include "Map/Entity.hpp"
#include "Map/Features.hpp"
#include "Model/Converter.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string_view>

namespace
{
auto NoError = "";
template <TWC::ContentType T>
void ListExcludedContent(TWC::ContentContainer<TWC::ContentRecordSet> &exclusions,
                         TWC::ContentContainer<TWC::ContentRecordVector> &candidates)
{
    using namespace TWC;
    const auto name = magic_enum::enum_name(T);
    if (candidates.HasContents<T>() &&
        ImGui::TreeNode(&exclusions.GetContents<T>(), "%.*s exclusions %zu/%zu", static_cast<int>(name.size()),
                        name.data(), exclusions.Count<T>(), candidates.Count<T>()))
    {
        for (const auto &id : candidates.GetContents<T>())
        {
            auto isExcluded = exclusions.GetContents<T>().contains(id);
            const auto label = std::format("{} #{}", name, id);
            if (ImGui::Checkbox(label.c_str(), &isExcluded))
            {
                if (isExcluded)
                    exclusions.GetContents<T>().emplace(id);
                else
                    exclusions.GetContents<T>().erase(id);
            }
        }
        ImGui::TreePop();
    }
}

template <TWC::ContentType T>
void InsertAll(TWC::ContentContainer<TWC::ContentRecordSet> &exclusions,
               TWC::ContentContainer<TWC::ContentRecordVector> &candidates,
               const TWC::MapEntity<TWC::DescribedContentCollection> &source)
{
    for (const auto &id : source.GetContents<T>())
    {
        auto record = TWC::Converter<T>::Serialize(id);
        const auto it = std::find(candidates.GetContents<T>().begin(), candidates.GetContents<T>().end(), record);
        if (it == candidates.GetContents<T>().end())
        {
            candidates.GetContents<T>().push_back(record);
            exclusions.GetContents<T>().emplace(record);
        }
    }
}

template <TWC::ContentType T> size_t GetHash(const TWC::ContentContainer<TWC::ContentRecordSet> &set)
{
    size_t hash = set.Count<T>();
    for (const auto &id : set.GetContents<T>())
        hash ^= std::hash<TWC::Record<T>>{}(id) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}
} // namespace

TWC::ConfigurableContentExclusions TWC::ConfigurableContentExclusions::Default()
{
    auto exclusions = ConfigurableContentExclusions{};
    exclusions.AutoExclusions.insert(ConfigurableExclusionReason::Exclusive_Gemstore);
    exclusions.AutoExclusions.insert(ConfigurableExclusionReason::Historical);
    return exclusions;
}

void TWC::ConfigurableContentExclusions::Render()
{
    if (!ExclusionCandidates)
    {
        ExclusionCandidates = ContentContainer<ContentRecordVector>{};

        ExclusionCandidates->GetContents<ContentType::HeroChallenge>() = {
            GetContents<ContentType::HeroChallenge>().begin(), GetContents<ContentType::HeroChallenge>().end()};
        ExclusionCandidates->GetContents<ContentType::RenownHeart>() = {GetContents<ContentType::RenownHeart>().begin(),
                                                                        GetContents<ContentType::RenownHeart>().end()};
        ExclusionCandidates->GetContents<ContentType::PointOfInterest>() = {
            GetContents<ContentType::PointOfInterest>().begin(), GetContents<ContentType::PointOfInterest>().end()};
        ExclusionCandidates->GetContents<ContentType::Vista>() = {GetContents<ContentType::Vista>().begin(),
                                                                  GetContents<ContentType::Vista>().end()};
        ExclusionCandidates->GetContents<ContentType::Waypoint>() = {GetContents<ContentType::Waypoint>().begin(),
                                                                     GetContents<ContentType::Waypoint>().end()};
    }
    if (ImGui::Button("Use addon defaults"))
    {
        *this = Default();
        return;
    }

    ImGui::Text("Automatically exclude from completion all content which is");
    static auto names = MakeExclusionNames();
    for (const auto value : magic_enum::enum_values<ConfigurableExclusionReason>())
    {
        auto isEnabled = AutoExclusions.contains(value);
        if (ImGui::Checkbox(names[value].data(), &isEnabled))
        {
            if (isEnabled)
                AutoExclusions.insert(value);
            else
                AutoExclusions.erase(value);
        }
    }
    ImGui::Separator();
    static uint32_t newId = 0;
    static std::string_view error = NoError;
    if (ImGui::InputScalar("Add Map by ID", ImGuiDataType_U32, &newId, nullptr, nullptr, nullptr,
                           ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (const auto content = G::Cache::Content->GetMapContent(newId))
        {
            InsertAll<ContentType::Vista>(*this, ExclusionCandidates.value(), content.value());
            InsertAll<ContentType::PointOfInterest>(*this, ExclusionCandidates.value(), content.value());
            InsertAll<ContentType::Waypoint>(*this, ExclusionCandidates.value(), content.value());
            InsertAll<ContentType::RenownHeart>(*this, ExclusionCandidates.value(), content.value());
            InsertAll<ContentType::HeroChallenge>(*this, ExclusionCandidates.value(), content.value());
            error = NoError;
            newId = 0;
        }
        else
            error = "Map#%u couldn't be found or has no discoverable content.";
    }
    if (error != NoError)
    {
        ImGui::TextColored({1, 0, 0, 1}, error.data(), newId);
    }

    ListExcludedContent<ContentType::HeroChallenge>(*this, ExclusionCandidates.value());
    ListExcludedContent<ContentType::RenownHeart>(*this, ExclusionCandidates.value());
    ListExcludedContent<ContentType::Waypoint>(*this, ExclusionCandidates.value());
    ListExcludedContent<ContentType::PointOfInterest>(*this, ExclusionCandidates.value());
    ListExcludedContent<ContentType::Vista>(*this, ExclusionCandidates.value());
}

magic_enum::containers::array<TWC::ConfigurableExclusionReason, std::string_view> TWC::ConfigurableContentExclusions::
    MakeExclusionNames()
{
    auto names = magic_enum::containers::array<ConfigurableExclusionReason, std::string_view>{};
    names.fill(""); /* TODO */
    names[ConfigurableExclusionReason::Historical] = "Historical/retired content which is no longer unlockable";
    names[ConfigurableExclusionReason::Exclusive_Gemstore] =
        "Content accessible exclusively through use of gemstore items";
    names[ConfigurableExclusionReason::Exclusive_Story] = "Content accessible exclusively through personal story";
    names[ConfigurableExclusionReason::Exclusive_Festival] =
        "Content accessible exclusively through seasonal festivals";
    names[ConfigurableExclusionReason::Exclusive_PvP] = "Player vs. Player maps";
    names[ConfigurableExclusionReason::Exclusive_WvW] = "World vs. World maps";
    return names;
}

size_t TWC::ConfigurableContentExclusions::GetStateHash() const
{
    size_t Hash = AutoExclusions.size();
    for (const auto &reason : AutoExclusions)
        Hash ^= static_cast<size_t>(reason) + 0x9e3779b9 + (Hash << 6) + (Hash >> 2);
    return Hash ^ GetHash<ContentType::HeroChallenge>(*this) ^ GetHash<ContentType::RenownHeart>(*this) ^
           GetHash<ContentType::Waypoint>(*this) ^ GetHash<ContentType::Vista>(*this) ^
           GetHash<ContentType::PointOfInterest>(*this);
}

std::unique_ptr<TWC::FilterInterface> TWC::ConfigurableContentExclusions::GetFilter() const
{
    auto filter = std::make_unique<FilterExcludedFeatures>();
    for (const auto &reason : AutoExclusions)
    {
        switch (reason)
        {
        case ConfigurableExclusionReason::Historical:
            filter->ForbidenContentFeatures.set(ContentFeatures::Retired);
            break;
        case ConfigurableExclusionReason::Exclusive_Gemstore:
            filter->ForbidenMapFeatures.set(MapFeatures::Accessible_Gemstore);
            break;
        case ConfigurableExclusionReason::Exclusive_Story:
            filter->ForbidenContentFeatures.set(ContentFeatures::StoryExclusive);
            break;
        case ConfigurableExclusionReason::Exclusive_Festival:
            filter->ForbidenMapFeatures.set(MapFeatures::Accessible_Festival);
            break;
        case ConfigurableExclusionReason::Exclusive_PvP:
            filter->ForbidenMapFeatures.set(MapFeatures::Competitive_PvP);
            break;
        case ConfigurableExclusionReason::Exclusive_WvW:
            filter->ForbidenMapFeatures.set(MapFeatures::Competitive_WvW);
            break;
        }
    }
    return filter;
}

void TWC::to_json(nlohmann::json &j, const ConfigurableContentExclusions &c)
{
    j["AutoExclusions"] = c.AutoExclusions;
    j["HeroChallenge"] = c.GetContents<ContentType::HeroChallenge>();
    j["RenownHeart"] = c.GetContents<ContentType::RenownHeart>();
    j["Waypoint"] = c.GetContents<ContentType::Waypoint>();
    j["PointOfInterest"] = c.GetContents<ContentType::PointOfInterest>();
    j["Vista"] = c.GetContents<ContentType::Vista>();
}

void TWC::from_json(const nlohmann::json &j, ConfigurableContentExclusions &c)
{
    j.at("AutoExclusions").get_to(c.AutoExclusions);
    j.at("HeroChallenge").get_to(c.GetContents<ContentType::HeroChallenge>());
    j.at("RenownHeart").get_to(c.GetContents<ContentType::RenownHeart>());
    j.at("Waypoint").get_to(c.GetContents<ContentType::Waypoint>());
    j.at("PointOfInterest").get_to(c.GetContents<ContentType::PointOfInterest>());
    j.at("Vista").get_to(c.GetContents<ContentType::Vista>());
}
