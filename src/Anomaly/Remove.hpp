#pragma once
#include "Constant/MapIdentifier.hpp"
#include "Constant/PointOfInterestRecord.hpp"
#include "Constant/WaypointRecord.hpp"
#include <tuple>

// This content was never released but is present in game's files as though it was accessible.
namespace TWC::AnomalyRemove
{
constexpr auto Maps = {
    ConstantMapIdentifier::VerdantBrinkClone,
    ConstantMapIdentifier::BastionOfThePenitentClone,
};
constexpr auto Waypoints = {
    std::tuple{ConstantMapIdentifier::AscalonianCatacombsExplorable, ConstantWaypointRecord::KashasTombWaypoint},
    {ConstantMapIdentifier::EdgeOfTheMists, ConstantWaypointRecord::MistsArenaSpectatorRallyPoint},
    {ConstantMapIdentifier::ToypocalypseParty, ConstantWaypointRecord::PlayerRespawn},
};
constexpr auto PointsOfInterest = {
    std::tuple{ConstantMapIdentifier::VictoryOrDeath, ConstantPointOfInterestRecord::ZhaitansLastStand},
};
} // namespace TWC::AnomalyRemove
