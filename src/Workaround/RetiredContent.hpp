#pragma once
#include <Constant/HeroChallengeRecord.hpp>
#include <Constant/MapIdentifier.hpp>
#include <Constant/PointOfInterestRecord.hpp>
#include <Constant/RenownHeartRecord.hpp>
#include <Constant/WaypointRecord.hpp>

// Content was discoverable historically,
// and is still present in game's dat file.
// Currently is not accessible
// - this does not include periodically available content ie. Festivals.
// - this also does not include maps that got retired entirely with all their content
namespace TWC
{
struct WorkaroundRetiredContent
{
    static constexpr auto Hearts = {
        ConstantRenownHeartRecord::AssistGrimarrMolesmasher,
        ConstantRenownHeartRecord::CapturePoints,
        ConstantRenownHeartRecord::FinishYourEnemies,
        ConstantRenownHeartRecord::ReviveAllies,
    };
    static constexpr auto Waypoints = {
        ConstantWaypointRecord::VaultWaypoint,
        ConstantWaypointRecord::MeadowsWaypoint,
        ConstantWaypointRecord::MoogoolooWaypoint,
    };
    static constexpr auto PointsOfInterest = {
        ConstantPointOfInterestRecord::PlazaOfPeace,
        ConstantPointOfInterestRecord::ForestOfAarden,
        ConstantPointOfInterestRecord::VerdanceFalls,
    };

    template <ContentType T> static consteval auto Get()
    {
        if constexpr (T == ContentType::Waypoint)
        {
            return Waypoints;
        }
        else if constexpr (T == ContentType::PointOfInterest)
        {
            return PointsOfInterest;
        }
        else if constexpr (T == ContentType::RenownHeart)
        {
            return Hearts;
        }
        else
            return std::initializer_list<Record<T>>{};
    }
};
} // namespace TWC