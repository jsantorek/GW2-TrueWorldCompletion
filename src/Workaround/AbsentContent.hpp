#pragma once
#include "Constant/MapIdentifier.hpp"
#include "Constant/PointOfInterestRecord.hpp"
#include "Constant/WaypointRecord.hpp"
#include "Content/Record.hpp"
#include "Content/Type.hpp"
#include <initializer_list>
#include <tuple>

// Content that - although present in game's dat file - for one reason or another
// is not, and never was discoverable.
namespace TWC::WorkaroundAbsentContent
{
constexpr auto Waypoints = {
    std::tuple{ConstantMapIdentifier::AscalonianCatacombsExplorable, ConstantWaypointRecord::KashasTombWaypoint},
    {ConstantMapIdentifier::EdgeOfTheMists, ConstantWaypointRecord::MistsArenaSpectatorRallyPoint},
    {ConstantMapIdentifier::ToypocalypsePublic, ConstantWaypointRecord::PlayerRespawn},
};

constexpr auto PointsOfInterest = {
    std::tuple{ConstantMapIdentifier::VictoryOrDeath, ConstantPointOfInterestRecord::ZhaitansLastStand},
};

template <ContentType T> consteval std::initializer_list<std::tuple<int, Record<T>>> Get()
{
    if constexpr (T == ContentType::Waypoint)
    {
        return Waypoints;
    }
    else if constexpr (T == ContentType::PointOfInterest)
    {
        return PointsOfInterest;
    }
    return {};
}
} // namespace TWC::WorkaroundAbsentContent
