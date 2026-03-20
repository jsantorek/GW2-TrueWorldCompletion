#include "Content/Cache.hpp"
#include "Anomaly/Remove.hpp"
#include "Anomaly/Repair.hpp"
#include "Constant/Undisclosed.hpp"
#include "Content/Descriptor.hpp"
#include "Content/Features.hpp"
#include "Content/Identifier.hpp"
#include "Content/Type.hpp"
#include "Game/Char/ChCliContext.h"
#include "Game/Content/ContentContext.h"
#include "Game/Content/EContentType.h"
#include "Game/Map/EMapFlags.h"
#include "Game/Map/MapDef.h"
#include "Game/MapLayout/MapLayoutMap.h"
#include "Game/MapLayout/MapLayoutPacingTask.h"
#include "Game/MapLayout/MapLayoutPointOfInterest.h"
#include "Game/PointOfInterest/EPoiType.h"
#include "Game/PointOfInterest/PointOfInterestDef.h"
#include "Game/PropContext.h"
#include "Game/Task/TaskSubRegionCommonDef.h"
#include "Logging.hpp"
#include "Map/Descriptor.hpp"
#include "model/Converter.hpp"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <> struct magic_enum::customize::enum_range<GW2RE::EMapFlags>
{
    static constexpr bool is_flags = true;
};
namespace
{
template <TWC::ContentFeatures Inclusion, typename ContentCollection, typename ExclusionIdentifier>
void ApplyExclusion(ContentCollection &contents, const std::set<ExclusionIdentifier> &excludedSet)
{
    for (auto &content : contents)
    {
        if (excludedSet.contains(static_cast<ExclusionIdentifier>(content)))
            content.Features.emplace(Inclusion);
        else
            content.Features.erase(Inclusion);
    }
}
inline TWC::Identifier<TWC::ContentType::RenownHeart> TaskIdentifier(GW2RE::TaskSubRegionCommonDef_t *task)
{
    return {static_cast<uint16_t>(task->ID), static_cast<uint16_t>(task->Progress ? task->Progress->ID : 0)};
}
inline TWC::Identifier<TWC::ContentType::HeroChallenge> ChallengeIdentifier(const GW2RE::SkillChallenge &challenge)
{
    return {static_cast<uint16_t>(challenge.Progress->ID), static_cast<uint16_t>(challenge.Bit)};
}
} // namespace

namespace TWC
{
template <ContentType T> using UnorderedIdentifiers = std::unordered_set<Identifier<T>>;
} // namespace TWC
template <> struct std::hash<TWC::Identifier<TWC::ContentType::HeroChallenge>>
{
    std::size_t operator()(const TWC::Identifier<TWC::ContentType::HeroChallenge> &id) const noexcept
    {
        auto hash = static_cast<size_t>(id.ProgressID);
        return (hash << sizeof(decltype(id.ProgressID))) + static_cast<size_t>(id.Bit);
    }
};
template <> struct std::hash<TWC::Identifier<TWC::ContentType::Vista>>
{
    std::size_t operator()(const TWC::Identifier<TWC::ContentType::Vista> &id) const noexcept
    {
        return static_cast<size_t>(id.ID);
    }
};
template <> struct std::hash<TWC::Identifier<TWC::ContentType::Waypoint>>
{
    std::size_t operator()(const TWC::Identifier<TWC::ContentType::Waypoint> &id) const noexcept
    {
        return static_cast<size_t>(id.ID);
    }
};
template <> struct std::hash<TWC::Identifier<TWC::ContentType::PointOfInterest>>
{
    std::size_t operator()(const TWC::Identifier<TWC::ContentType::PointOfInterest> &id) const noexcept
    {
        return static_cast<size_t>(id.ID);
    }
};
template <> struct std::hash<TWC::Identifier<TWC::ContentType::RenownHeart>>
{
    std::size_t operator()(const TWC::Identifier<TWC::ContentType::RenownHeart> &id) const noexcept
    {
        return static_cast<size_t>(id.ID);
    }
};

void TWC::ContentCache::Initialize()
{
    using namespace GW2RE;

    CContentCtx ctx = CPropContext::Get()->ContentCtx;

    std::map<uint32_t, MapEntity<UnorderedIdentifiers>> Buffer;
    auto mapLayouts = ctx.GetContentStream<MapLayoutMap_t>(EContentType::MapLayoutMap);
    while (auto layout = mapLayouts.next())
    {
        if (!layout->Data || !layout->Data->Def)
            continue;
        if (layout->PointsOfInterest.empty() && layout->Tasks.empty() && layout->HeroChallenges.empty())
            continue;
        const auto def = layout->Data->Def;
        auto mapIt = Buffer.find(def->ID);
        auto &[_, map] = mapIt == Buffer.end() ? *Buffer.emplace(def->ID, def).first : *mapIt;
        for (auto task : layout->Tasks)
        {
            map.Hearts.insert(TaskIdentifier(task->Def));
        }
        for (auto poi : layout->PointsOfInterest)
        {
            switch (poi->Def->Type)
            {
            case GW2RE::EPoiType::PointOfInterest:
                map.PointsOfInterest.emplace(poi->Def->ID);
                break;
            case GW2RE::EPoiType::Waypoint:
                map.Waypoints.emplace(poi->Def->ID);
                break;
            case GW2RE::EPoiType::Vista:
                map.Vistas.emplace(poi->Def->ID);
                break;
            default:
                break;
            }
        }
    }

    auto const mapWhitelist = std::unordered_set<uint32_t>(AnomalyRepair::Maps.begin(), AnomalyRepair::Maps.end());
    auto landmarks = ctx.GetContentStream<PointOfInterestDef_t>(EContentType::PointOfInterestDef);
    while (auto landmark = landmarks.next())
    {
        if (!landmark->Map || landmark->Type == EPoiType::Unlock)
            continue;
        if (!mapWhitelist.contains(landmark->Map->ID))
            continue;
        auto mapIt = Buffer.find(landmark->Map->ID);
        auto &[_, map] = mapIt == Buffer.end() ? *Buffer.emplace(landmark->Map->ID, landmark->Map).first : *mapIt;
        switch (landmark->Type)
        {
        case GW2RE::EPoiType::PointOfInterest:
            map.PointsOfInterest.emplace(landmark->ID);
            break;
        case GW2RE::EPoiType::Waypoint:
            map.Waypoints.emplace(landmark->ID);
            break;
        case GW2RE::EPoiType::Vista:
            map.Vistas.emplace(landmark->ID);
            break;
        default:
            break;
        }
    }
    const std::unordered_set<uint32_t> HeroChallengeProgression(
        ConstantUndisclosed::HeroChallengeProgressionMapping.begin(),
        ConstantUndisclosed::HeroChallengeProgressionMapping.end());
    // best method to get released skill challenges is to fetch CharCliCtx cache
    constexpr auto LayoutMissing = 0xFFFFFFFF;
    for (auto [_, challenge] : CPropContext::Get()->CharCliCtx->SkillChallenges)
    {
        if (challenge.MapLayoutIndex == LayoutMissing)
        {
            continue;
        }
        const auto id = ChallengeIdentifier(challenge);
        if (auto mapId = AnomalyRepair::HasBorderlandsAnalogue(challenge.Map->ID))
        {
            if (auto mapIt = Buffer.find(mapId); mapIt == Buffer.end())
            {
                /* TODO */ LOG_DEBUG();
            }
            else
            {
                mapIt->second.Challenges.emplace(id);
            }
        }
        auto mapIt = Buffer.find(challenge.Map->ID);
        auto &[_, map] = mapIt == Buffer.end() ? *Buffer.emplace(challenge.Map->ID, challenge.Map).first : *mapIt;
        map.Challenges.emplace(id);
        if (!challenge.Progress || !HeroChallengeProgression.contains(challenge.Progress->ID))
        {
            /* TODO */ LOG_DEBUG();
        }
    }

    auto tasks = ctx.GetContentStream<TaskSubRegionCommonDef_t>(EContentType::TaskSubRegionCommonDef);
    while (auto task = tasks.next())
    {
        auto it = std::find_if(AnomalyRepair::Hearts.begin(), AnomalyRepair::Hearts.end(),
                               [id = task->ID](const auto &map_task) { return std::get<1>(map_task) == id; });
        if (it != AnomalyRepair::Hearts.end())
        {
            auto map = Buffer.find(std::get<0>(*it));
            if (map != Buffer.end())
            {
                map->second.Hearts.emplace(TaskIdentifier(task));
            }
            else
            {
                /* TODO */ LOG_DEBUG();
            }
        }
        HeartProgressionMapping.emplace(static_cast<uint16_t>(task->ID),
                                        task->Progress ? static_cast<uint16_t>(task->Progress->ID) : 0);
    }
    for (const auto &[mapId, challengeRecord] : AnomalyRepair::HeroChallenges)
    {
        auto mapIt = Buffer.find(mapId);

        if (mapIt == Buffer.end())
        {
            /* TODO */ LOG_DEBUG();
            continue;
        }
        mapIt->second.Challenges.emplace(Converter<ContentType::HeroChallenge>::Deserialize(challengeRecord));
    }

    for (const auto &[mapId, landmark] : AnomalyRepair::Waypoints)
    {
        auto map = Buffer.find(mapId);
        if (map != Buffer.end())
        {
            map->second.Waypoints.emplace(Converter<ContentType::Waypoint>::Deserialize(landmark));
        }
        else
        {
            /* TODO */ LOG_DEBUG();
        }
    }

    for (const auto &[mapId, landmark] : AnomalyRepair::PointsOfInterest)
    {
        auto map = Buffer.find(mapId);
        if (map != Buffer.end())
        {
            map->second.PointsOfInterest.emplace(Converter<ContentType::PointOfInterest>::Deserialize(landmark));
        }
        else
        {
            /* TODO */ LOG_DEBUG();
        }
    }

    for (const auto &mapId : AnomalyRemove::Maps)
    {
        Buffer.erase(mapId);
    }

    for (const auto &[mapId, landmark] : AnomalyRemove::Waypoints)
    {
        auto map = Buffer.find(mapId);
        if (map != Buffer.end())
        {
            map->second.Waypoints.erase(Converter<ContentType::Waypoint>::Deserialize(landmark));
        }
        else
        {
            /* TODO */ LOG_DEBUG();
        }
    }

    for (const auto &[mapId, landmark] : AnomalyRemove::PointsOfInterest)
    {
        auto map = Buffer.find(mapId);
        if (map != Buffer.end())
        {
            map->second.PointsOfInterest.erase(Converter<ContentType::PointOfInterest>::Deserialize(landmark));
        }
        else
        {
            /* TODO */ LOG_DEBUG();
        }
    }

    Maps.reserve(Buffer.size());
    for (const auto &[_, bufferedMap] : Buffer)
    {
        if (bufferedMap.Definition == nullptr)
        {
            /* TODO */ LOG_DEBUG();
            continue;
        }
        auto &map = Maps.emplace_back(bufferedMap.Definition);
        const auto descriptor = ContentDescriptor{.Features = {}, .Map = static_cast<MapDescriptor>(map)};
        map.Challenges.reserve(bufferedMap.Count<ContentType::HeroChallenge>());
        for (const auto id : bufferedMap.GetContents<ContentType::HeroChallenge>())
        {
            auto it = std::find_if(Challenges.begin(), Challenges.end(), [&](auto c) {
                return id == static_cast<Identifier<ContentType::HeroChallenge>>(c);
            });
            if (it == Challenges.end())
            {
                map.Challenges.emplace_back(Count<ContentType::HeroChallenge>());
                Challenges.emplace_back(descriptor, id);
            }
            else
            {
                if (it->Map.Features != map.Features)
                {
                    // LOG_DEBUG(); /* TODO */
                    LOG(DEBUG, "Map#{} assigned Challenge({}-{}) descriptor [{:X}:{}] -> [{:X}:{}]", map.Definition->ID,
                        id.ProgressID, id.Bit, it->Map.Features.underlying_value(), static_cast<int>(it->Map.Expansion),
                        map.Features.underlying_value(), static_cast<int>(map.Expansion));
                    it->Map.Features |= map.Features;
                }
                const auto ref = std::distance(Challenges.begin(), it);
                map.Challenges.emplace_back(ref);
            }
        }

        map.Hearts.reserve(bufferedMap.Count<ContentType::RenownHeart>());
        for (const auto id : bufferedMap.GetContents<ContentType::RenownHeart>())
        {
            const auto it = std::find_if(Hearts.begin(), Hearts.end(), [&](auto c) {
                return id == static_cast<Identifier<ContentType::RenownHeart>>(c);
            });
            if (it == Hearts.end())
            {
                map.Hearts.emplace_back(Count<ContentType::RenownHeart>());
                Hearts.emplace_back(descriptor, id);
            }
            else
            {
                if (it->Map.Features != map.Features)
                {
                    // LOG_DEBUG(); /* TODO */
                    LOG(DEBUG, "Map#{} assigned Task#{} descriptor [{:X}:{}] -> [{:X}:{}]", map.Definition->ID, id.ID,
                        it->Map.Features.underlying_value(), static_cast<int>(it->Map.Expansion),
                        map.Features.underlying_value(), static_cast<int>(map.Expansion));
                    it->Map.Features |= map.Features;
                }
                const auto ref = std::distance(Hearts.begin(), it);
                map.Hearts.emplace_back(ref);
            }
        }

        map.PointsOfInterest.reserve(bufferedMap.Count<ContentType::PointOfInterest>());
        for (const auto id : bufferedMap.GetContents<ContentType::PointOfInterest>())
        {
            const auto it = std::find_if(PointsOfInterest.begin(), PointsOfInterest.end(), [&](auto c) {
                return static_cast<Identifier<ContentType::Landmark>>(id) ==
                       static_cast<Identifier<ContentType::Landmark>>(c);
            });
            if (it == GetContents<ContentType::PointOfInterest>().end())
            {
                map.PointsOfInterest.emplace_back(Count<ContentType::PointOfInterest>());
                PointsOfInterest.emplace_back(descriptor, id);
            }
            else
            {
                if (it->Map.Features != map.Features)
                {
                    // LOG_DEBUG(); /* TODO */
                    LOG(DEBUG, "Map#{} assigned Poi#{} descriptor [{:X}:{}] -> [{:X}:{}]", map.Definition->ID, id.ID,
                        it->Map.Features.underlying_value(), static_cast<int>(it->Map.Expansion),
                        map.Features.underlying_value(), static_cast<int>(map.Expansion));
                    it->Map.Features |= map.Features;
                }
                const auto ref = std::distance(PointsOfInterest.begin(), it);
                map.PointsOfInterest.emplace_back(ref);
            }
        }

        map.Vistas.reserve(bufferedMap.Count<ContentType::Vista>());
        for (const auto id : bufferedMap.GetContents<ContentType::Vista>())
        {
            const auto it = std::find_if(Vistas.begin(), Vistas.end(), [&](auto c) {
                return static_cast<Identifier<ContentType::Landmark>>(id) ==
                       static_cast<Identifier<ContentType::Landmark>>(c);
            });
            if (it == GetContents<ContentType::Vista>().end())
            {
                map.Vistas.emplace_back(Count<ContentType::Vista>());
                Vistas.emplace_back(descriptor, id);
            }
            else
            {
                if (it->Map.Features != map.Features)
                {
                    // LOG_DEBUG(); /* TODO */
                    LOG(DEBUG, "Map#{} assigned Vista#{} descriptor [{:X}:{}] -> [{:X}:{}]", map.Definition->ID, id.ID,
                        it->Map.Features.underlying_value(), static_cast<int>(it->Map.Expansion),
                        map.Features.underlying_value(), static_cast<int>(map.Expansion));
                    it->Map.Features |= map.Features;
                }
                const auto ref = std::distance(Vistas.begin(), it);
                map.Vistas.emplace_back(ref);
            }
        }

        map.Waypoints.reserve(bufferedMap.Count<ContentType::Waypoint>());
        for (const auto id : bufferedMap.GetContents<ContentType::Waypoint>())
        {
            const auto it = std::find_if(Waypoints.begin(), Waypoints.end(), [&](auto c) {
                return static_cast<Identifier<ContentType::Landmark>>(id) ==
                       static_cast<Identifier<ContentType::Landmark>>(c);
            });
            if (it == GetContents<ContentType::Waypoint>().end())
            {
                map.Waypoints.emplace_back(Count<ContentType::Waypoint>());
                Waypoints.emplace_back(descriptor, id);
            }
            else
            {
                if (it->Map.Features != map.Features)
                {
                    // LOG_DEBUG(); /* TODO */
                    LOG(DEBUG, "Map#{} assigned Waypoint#{} descriptor [{:X}:{}] -> [{:X}:{}]", map.Definition->ID,
                        id.ID, it->Map.Features.underlying_value(), static_cast<int>(it->Map.Expansion),
                        map.Features.underlying_value(), static_cast<int>(map.Expansion));
                    it->Map.Features |= map.Features;
                }
                const auto ref = std::distance(Waypoints.begin(), it);
                map.Waypoints.emplace_back(ref);
            }
        }
    }
    Challenges.shrink_to_fit();
    Hearts.shrink_to_fit();
    PointsOfInterest.shrink_to_fit();
    Vistas.shrink_to_fit();
    Waypoints.shrink_to_fit();
}

void TWC::ContentCache::Update(std::set<Identifier<ContentType::RenownHeart>> excludedHearts,
                               std::set<Identifier<ContentType::HeroChallenge>> excludedChallenges,
                               std::set<Identifier<ContentType::Landmark>> excludedLandmarks)
{
    ApplyExclusion<ContentFeatures::UserExcluded>(Hearts, excludedHearts);
    ApplyExclusion<ContentFeatures::UserExcluded>(Challenges, excludedChallenges);
    ApplyExclusion<ContentFeatures::UserExcluded>(Waypoints, excludedLandmarks);
    ApplyExclusion<ContentFeatures::UserExcluded>(Vistas, excludedLandmarks);
    ApplyExclusion<ContentFeatures::UserExcluded>(PointsOfInterest, excludedLandmarks);
}

std::optional<uint16_t> TWC::ContentCache::GetHeartProgressionId(uint32_t id) const
{
    const auto it = HeartProgressionMapping.find(id);
    if (it == HeartProgressionMapping.end())
        return std::nullopt;
    return it->second;
}

bool TWC::ContentCache::HasMap(uint32_t id) const
{
    const auto it = std::lower_bound(Maps.begin(), Maps.end(), id,
                                     [](const auto &item, uint32_t mapId) { return item.Definition->ID < mapId; });
    return it != Maps.end() && (*it).Definition->ID == id;
}

std::optional<TWC::MapDescriptor> TWC::ContentCache::GetMapDescriptor(uint32_t id) const
{
    const auto it = std::lower_bound(Maps.begin(), Maps.end(), id,
                                     [](const auto &item, uint32_t mapId) { return item.Definition->ID < mapId; });
    if (it == Maps.end() || (*it).Definition->ID != id)
        return std::nullopt;
    return static_cast<TWC::MapDescriptor>(*it);
}

bool TWC::ContentCache::HasMapContent(uint32_t id) const
{
    const auto it = std::lower_bound(Maps.begin(), Maps.end(), id,
                                     [](const auto &item, uint32_t mapId) { return item.Definition->ID < mapId; });
    if (it == Maps.end() || (*it).Definition->ID != id)
        return false;
    return !it->Hearts.empty() || !it->Challenges.empty() || !it->Waypoints.empty() || !it->Vistas.empty() ||
           !it->PointsOfInterest.empty();
}

std::vector<GW2RE::MapDef_t *> TWC::ContentCache::GetMapDefinitions() const
{
    std::vector<GW2RE::MapDef_t *> maps;
    maps.reserve(Maps.size());
    for (const auto &map : Maps)
        maps.emplace_back(map.Definition);
    return maps;
}

std::optional<TWC::MapEntity<TWC::DescribedContentCollection>> TWC::ContentCache::GetMapContent(uint32_t id) const
{
    auto it = std::lower_bound(Maps.begin(), Maps.end(), id,
                               [](const auto &item, uint32_t mapId) { return item.Definition->ID < mapId; });
    if (it == Maps.end() || (*it).Definition->ID != id)
        return std::nullopt;

    MapEntity<DescribedContentCollection> map(it->Definition);
    std::for_each(it->Hearts.begin(), it->Hearts.end(),
                  [&](const auto &heart) { map.Hearts.emplace_back(Hearts[heart]); });
    std::for_each(it->Challenges.begin(), it->Challenges.end(),
                  [&](const auto &challenge) { map.Challenges.emplace_back(Challenges[challenge]); });
    std::for_each(it->Waypoints.begin(), it->Waypoints.end(),
                  [&](const auto &waypoint) { map.Waypoints.emplace_back(Waypoints[waypoint]); });
    std::for_each(it->Vistas.begin(), it->Vistas.end(),
                  [&](const auto &vista) { map.Vistas.emplace_back(Vistas[vista]); });
    std::for_each(it->PointsOfInterest.begin(), it->PointsOfInterest.end(),
                  [&](const auto &poi) { map.PointsOfInterest.emplace_back(PointsOfInterest[poi]); });
    return map;
}
