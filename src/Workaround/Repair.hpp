#pragma once
#include "Model/Continent.hpp"
#include "Model/Festival.hpp"
#include <Constant/HeroChallengeRecord.hpp>
#include <Constant/MapIdentifier.hpp>
#include <Constant/PointOfInterestRecord.hpp>
#include <Constant/RenownHeartRecord.hpp>
#include <Constant/WaypointRecord.hpp>

// There are some contents that, although discoverable (either currently or historically)
// are not present in game in a way that would allow association with a map.
// For those to be accounted for in completionc omputation,
// they need to be manually specified and associated with a map.
namespace TWC::AnomalyRepair
{
// for unknown reasons those maps are missing region
constexpr auto GetContinent(uint32_t MapId) -> Continent
{
    switch (MapId)
    {
    case ConstantMapIdentifier::Falooaloo:
    case ConstantMapIdentifier::DiscoveringScarletsBreakthrough:
    case ConstantMapIdentifier::IntoTheLabyrinth:
    case ConstantMapIdentifier::EnemyOfMyEnemy:
    case ConstantMapIdentifier::EnemyOfMyEnemyTheBeastmarshal:
    case ConstantMapIdentifier::EnemyOfMyEnemyTheTroopmarshal:
    case ConstantMapIdentifier::MountBalriorLabel:
    case ConstantMapIdentifier::BastionOfThePenitentLabel:
        return Continent::Tyria;
    default:
        break;
    }
    return Continent::None;
}

// for unknown reasons those maps are missing region
constexpr auto GetFestival(uint32_t MapId) -> Festival
{
    switch (MapId)
    {
    case ConstantMapIdentifier::ToypocalypsePublic:
    case ConstantMapIdentifier::SnowballMayhem:
    case ConstantMapIdentifier::TixxsInfinirarium:
        return Festival::Wintersday;
    case ConstantMapIdentifier::ReapersRumble:
    case ConstantMapIdentifier::AscentToMadness:
    case ConstantMapIdentifier::MadKingsLabyrinth:
    case ConstantMapIdentifier::MadKingsRaceway:
        return Festival::Halloween;
    case ConstantMapIdentifier::DragonBallArena:
        return Festival::LunarNewYear;
    case ConstantMapIdentifier::AspectArena:
    case ConstantMapIdentifier::LabyrinthineCliffs:
    case ConstantMapIdentifier::TheCrownPavilion:
        return Festival::FestivalOfTheFourWinds;
    default:
        break;
    }
    return Festival::None;
}

constexpr bool IsGemstoreExclusive(uint32_t MapId)
{
    switch (MapId)
    {
    case ConstantMapIdentifier::MistlockSanctuary:
    case ConstantMapIdentifier::ArmisticeBastion:
    case ConstantMapIdentifier::ThousandSeasPavilion:
        return true;
    default:
        break;
    }
    return false;
}
} // namespace TWC::AnomalyRepair

/* TODO
Map#1098 Claiming Gilded Hollow
*/