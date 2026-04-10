#pragma once
#include "Constant/RenownHeartRecord.hpp"
#include "Constant/WaypointRecord.hpp"
#include <Constant/PointOfInterestRecord.hpp>

namespace TWC
{
struct ConstantStoryContent
{
    static constexpr auto PointsOfInterest = {
        ConstantPointOfInterestRecord::TheMidnightThrone,
        ConstantPointOfInterestRecord::ReliquaryOfTheChosen,
        ConstantPointOfInterestRecord::EasternPyramid,
        ConstantPointOfInterestRecord::WesternPyramid,
        ConstantPointOfInterestRecord::LionsTorch,
        ConstantPointOfInterestRecord::MawAndPride,
        ConstantPointOfInterestRecord::AlterOfSecrets,
        ConstantPointOfInterestRecord::DraconicBattery,
        ConstantPointOfInterestRecord::SalvageOfTheWhitebearsPride,
        ConstantPointOfInterestRecord::PactEncampment,
        ConstantPointOfInterestRecord::SalvageOfTheRiannocsDream,
        ConstantPointOfInterestRecord::SalvageOfTheLadyLivia,
        ConstantPointOfInterestRecord::RoguesLanding,
    };

    static constexpr auto Hearts = {
        ConstantRenownHeartRecord::HelpAksimStory,
    };

    static constexpr auto Waypoints = {
        ConstantWaypointRecord::PactCampWaypoint,         ConstantWaypointRecord::TheHumbleWaypoint,
        ConstantWaypointRecord::TheGloryOfTyriasWaypoint, ConstantWaypointRecord::CavernEntranceWaypoint,
        ConstantWaypointRecord::OverlookWaypoint,         ConstantWaypointRecord::OreProcessingWaypoint,
        ConstantWaypointRecord::RoughneckCrookWaypoint,   ConstantWaypointRecord::KleptotronicAdvancedDesignsWaypoint,
        ConstantWaypointRecord::ScoundrelsHovelWaypoint,
    };

    template <ContentType T> static consteval auto Get()
    {
        if constexpr (T == ContentType::PointOfInterest)
        {
            return PointsOfInterest;
        }
        else if constexpr (T == ContentType::RenownHeart)
        {
            return Hearts;
        }
        else if constexpr (T == ContentType::Waypoint)
        {
            return Waypoints;
        }
        else
            return std::initializer_list<Record<T>>{};
    }
};
} // namespace TWC
