#include "options/ExclusionsConfigurator.hpp"
#include "Content/Type.hpp"
#include "model/ContentDescriptor.hpp"
#include "model/Converter.hpp"
#include "options/Exclusions.hpp"
#include <Content/Cache.hpp>
#include <algorithm>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <memory>

using namespace TWC;

void TWC::ExclusionsConfigurator::AddUnlistedExclusions()
{
    auto custom = MapExclusion{};
    for (const auto id : Pois)
        if (std::none_of(Maps.begin(), Maps.end(), [&](const auto &map) { return map.Contains(id, Pois); }))
            custom.With<ContentType::Landmark>(id, Pois); // we no longer know if its vista, poi or waypoint
    for (const auto id : Tasks)
        if (std::none_of(Maps.begin(), Maps.end(), [&](const auto &map) { return map.Contains(id, Tasks); }))
            custom.With<ContentType::RenownHeart>(id, Tasks);
    for (const auto id : Challanges)
        if (std::none_of(Maps.begin(), Maps.end(), [&](const auto &map) { return map.Contains(id, Challanges); }))
            custom.With<ContentType::HeroChallenge>(id, Challanges);
    Maps.emplace_back(std::move(custom));
}

TWC::ExclusionsConfigurator::ExclusionsConfigurator(std::set<Record<ContentType::RenownHeart>> &tasks,
                                                    std::set<Record<ContentType::Landmark>> &pois,
                                                    std::set<Record<ContentType::HeroChallenge>> &challanges)
    : Tasks(tasks), Pois(pois), Challanges(challanges)
{
    AddExplicitExclusions();
    AddUnlistedExclusions();
}

std::vector<std::tuple<ContentType, std::string, uint32_t, ExclusionReason>> Temp;
void TWC::ExclusionsConfigurator::AddExplicitExclusions()
{
    Maps.emplace_back(1206, "Mistlock Sanctuary")
        .With<ContentType::Waypoint>(2547, Pois, ExclusionReason::Exclusive_Gemstore);

    Maps.emplace_back(1315, "Armistice Bastion")
        .With<ContentType::Waypoint>(3013, Pois, ExclusionReason::Exclusive_Gemstore)
        .With<ContentType::Waypoint>(3014, Pois, ExclusionReason::Exclusive_Gemstore)
        .With<ContentType::Waypoint>(3015, Pois, ExclusionReason::Exclusive_Gemstore);

    Maps.emplace_back(1465, "Thousand Seas Pavilion")
        .With<ContentType::Waypoint>(3449, Pois, ExclusionReason::Exclusive_Gemstore);

    Maps.emplace_back(335, "Claw Island")
        .With<ContentType::PointOfInterest>(1229, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1274, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1275, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1276, Pois, ExclusionReason::Exclusive_Story);

    Maps.emplace_back(775, "Temple of the Forgotten God")
        .With<ContentType::PointOfInterest>(1282, Pois, ExclusionReason::Exclusive_Story);

    Maps.emplace_back(111, "Victory or Death")
        .With<ContentType::Waypoint>(1631, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::Waypoint>(1632, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::Waypoint>(859, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1559, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1560, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1561, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(1562, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(862, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(863, Pois, ExclusionReason::Inaccessible);

    Maps.emplace_back(901, "Molten Furnace")
        .With<ContentType::Waypoint>(1799, Pois, ExclusionReason::Historical)
        .With<ContentType::Waypoint>(1800, Pois, ExclusionReason::Historical)
        .With<ContentType::Waypoint>(1801, Pois, ExclusionReason::Historical);

    Maps.emplace_back(915, "Aetherblade Retreat")
        .With<ContentType::Waypoint>(1807, Pois, ExclusionReason::Historical)
        .With<ContentType::Waypoint>(1808, Pois, ExclusionReason::Historical)
        .With<ContentType::Waypoint>(1810, Pois, ExclusionReason::Historical);

    Maps.emplace_back(938, "The Reliquary").With<ContentType::Waypoint>(1836, Pois, ExclusionReason::Historical);

    Maps.emplace_back(939, "Resealing the Bloody Prince")
        .With<ContentType::Waypoint>(1835, Pois, ExclusionReason::Historical);

    Maps.emplace_back(880, "Toypocalypse - solo")
        .With<ContentType::Waypoint>(1763, Pois, ExclusionReason::Seasonal_Wintersday);
    Maps.emplace_back(1270, "Toypocalypse - party")
        .With<ContentType::Waypoint>(2849, Pois, ExclusionReason::Historical);

    Maps.emplace_back(877, "Snowball Mayhem")
        .With<ContentType::Waypoint>(1760, Pois, ExclusionReason::Seasonal_Wintersday)
        .With<ContentType::Waypoint>(1761, Pois, ExclusionReason::Seasonal_Wintersday);

    Maps.emplace_back(878, "Tixxs Infinirarium")
        .With<ContentType::Waypoint>(1762, Pois, ExclusionReason::Seasonal_Wintersday);

    Maps.emplace_back(862, "Reapers Rumble")
        .With<ContentType::Waypoint>(1742, Pois, ExclusionReason::Seasonal_Halloween)
        .With<ContentType::Waypoint>(1743, Pois, ExclusionReason::Seasonal_Halloween);

    Maps.emplace_back(863, "Ascent to Madness")
        .With<ContentType::Waypoint>(1740, Pois, ExclusionReason::Seasonal_Halloween);

    Maps.emplace_back(866, "Mad King's Labyrinth")
        .With<ContentType::Waypoint>(1741, Pois, ExclusionReason::Seasonal_Halloween)
        .With<ContentType::Waypoint>(1834, Pois, ExclusionReason::Seasonal_Halloween);

    Maps.emplace_back(1304, "Mad King's Raceway")
        .With<ContentType::Waypoint>(2971, Pois, ExclusionReason::Seasonal_Halloween);

    Maps.emplace_back(911, "Dragon Ball Arena")
        .With<ContentType::Waypoint>(1803, Pois, ExclusionReason::Seasonal_LunarNewYear)
        .With<ContentType::Waypoint>(1804, Pois, ExclusionReason::Seasonal_LunarNewYear);

    Maps.emplace_back(918, "Aspect Arena")
        .With<ContentType::Waypoint>(1818, Pois, ExclusionReason::Seasonal_FestivalOfTheFourWinds)
        .With<ContentType::Waypoint>(1819, Pois, ExclusionReason::Seasonal_FestivalOfTheFourWinds);

    Maps.emplace_back(922, "Labyrinthine Cliffs")
        .With<ContentType::Waypoint>(1820, Pois, ExclusionReason::Seasonal_FestivalOfTheFourWinds)
        .With<ContentType::Waypoint>(1821, Pois, ExclusionReason::Seasonal_FestivalOfTheFourWinds);

    Maps.emplace_back(929, "The Crown Pavilion")
        .With<ContentType::Waypoint>(1823, Pois, ExclusionReason::Seasonal_FestivalOfTheFourWinds);

    Maps.emplace_back(935, "Hub").With<ContentType::Waypoint>(1824, Pois, ExclusionReason::Seasonal_SuperAdventureBox);

    Maps.emplace_back(36, "Ascalonian Catacombs (Story)")
        .With<ContentType::Waypoint>(942, Pois, ExclusionReason::Inaccessible);

    Maps.emplace_back(1042, "Verdant Brink - inaccessible")
        .With<ContentType::Waypoint>(1984, Pois, ExclusionReason::Inaccessible);

    Maps.emplace_back(968, "Edge of the Mists").With<ContentType::Waypoint>(3204, Pois, ExclusionReason::Inaccessible);

    Maps.emplace_back(1217, "Sparking the Flame")
        .With<ContentType::RenownHeart>(414, Tasks, ExclusionReason::Exclusive_Story);

    Maps.emplace_back(1526, "Inner Nayos")
        .With<ContentType::PointOfInterest>(3725, Pois, ExclusionReason::Exclusive_Story)
        .With<ContentType::PointOfInterest>(3726, Pois, ExclusionReason::Exclusive_Story);

    // 183-186, 357-362, 392, 399, 402, 417, 432
    Maps.emplace_back(0, "Additional")
        .With<ContentType::RenownHeart>(58, Tasks, ExclusionReason::Historical)
        .With<ContentType::RenownHeart>(81, Tasks, ExclusionReason::Historical)
        .With<ContentType::RenownHeart>(82, Tasks, ExclusionReason::Historical)
        .With<ContentType::RenownHeart>(83, Tasks, ExclusionReason::Historical);
}

void TWC::ExclusionsConfigurator::Render()
{
    DrawMultiToggles();
    ImGui::Separator();
    ImGui::Text("Direct control over content of each map excluded from world completion computation");
    DrawMapDetails();

    ImGui::Separator();
    ImGui::Text("Exclude other map's content from completion");
    DrawMapAddition();
}
void TWC::ExclusionsConfigurator::DrawMultiToggles()
{
    constexpr auto offset = static_cast<uint32_t>(ExclusionReason::Count);
    uint16_t Counter[2 * offset] = {0};
    for (const auto &exclusions : Maps)
    {
        for (const auto &exclusion : exclusions.Exclusions)
        {
            const int index = static_cast<uint32_t>(exclusion->Reason);
            Counter[index]++;
            if (exclusion->IsEnabled())
                Counter[index + offset]++;
        }
    }
    uint32_t Flags = 0;
    for (uint32_t i = 0; i < offset; i++)
    {
        if (!Counter[i])
            continue;
        auto Shift = 2 * i;
        auto NewFlags = static_cast<uint32_t>(Counter[i] == Counter[i + offset]) * 2 +
                        static_cast<uint32_t>(Counter[i + offset] != 0);
        Flags |= NewFlags << Shift;
        auto Mask = 0b11 << Shift;
        if (ImGui::CheckboxFlags(magic_enum::enum_name(static_cast<ExclusionReason>(i)).data(), &Flags, Mask))
        {
            for (const auto &map : Maps)
                for (const auto &exclusion : map.Exclusions)
                    if (exclusion->Reason == static_cast<ExclusionReason>(i))
                        exclusion->SetEnabled(Flags & Mask);
        }
    }
}
void TWC::ExclusionsConfigurator::DrawMapDetails()
{
    for (const auto &map : Maps)
    {
        if (!map.Exclusions.empty() && ImGui::TreeNode(map.Description.c_str()))
        {
            for (const auto &exclusion : map.Exclusions)
            {
                bool isEnabled = exclusion->IsEnabled();
                if (ImGui::Checkbox(exclusion->Description.c_str(), &isEnabled))
                    exclusion->SetEnabled(isEnabled);
            }
            ImGui::TreePop();
        }
    }
}

void TWC::ExclusionsConfigurator::DrawIgnoreButtons()
{
    // ImGui::SameLine();
    // if (ImGui::Button("Ignore PvP"))
    //     ExcludeContent(Retired::ContentFeature::MODE_PvP);
    // ImGui::SameLine();
    // if (ImGui::Button("Ignore WvW"))
    //     ExcludeContent(Retired::ContentFeature::MODE_WvW);
    // ->OldContent
}

void TWC::ExclusionsConfigurator::DrawMapAddition()
{
    static uint32_t newId = 0;
    static std::string error = "";
    if (ImGui::InputScalar("Add Map by ID", ImGuiDataType_U32, &newId, nullptr, nullptr, nullptr,
                           ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (const auto content = G::Cache::Content->GetMapContent(newId); !content)
        {
            error = "Map#%u couldn't be found or has no discoverable content.";
        }
        else if (std::any_of(Maps.begin(), Maps.end(), [&](const auto &map) { return map.ID == newId; }))
        {
            error = "Map#%u already listed.";
        }
        else
        {
            auto &exclusion = Maps.emplace_back(newId);
            for (auto task : content->Hearts)
                exclusion.With<ContentType::RenownHeart>(Converter<ContentType::RenownHeart>::Serialize(task), Tasks);
            for (auto lmk : content->PointsOfInterest)
                exclusion.With<ContentType::PointOfInterest>(Converter<ContentType::PointOfInterest>::Serialize(lmk),
                                                             Pois);
            for (auto wpt : content->Waypoints)
                exclusion.With<ContentType::Waypoint>(Converter<ContentType::Waypoint>::Serialize(wpt), Pois);
            for (auto vst : content->Vistas)
                exclusion.With<ContentType::Vista>(Converter<ContentType::Vista>::Serialize(vst), Pois);
            for (auto identifier : content->Challenges)
                exclusion.With<ContentType::HeroChallenge>(Converter<ContentType::HeroChallenge>::Serialize(identifier),
                                                           Challanges);
            newId = 0;
            error = "";
        }
    }
    if (!error.empty())
    {
        ImGui::TextColored({1, 0, 0, 1}, error.c_str(), newId);
    }
}
