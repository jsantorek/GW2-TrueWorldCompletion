#pragma once
#include <Constant/HeroChallengeRecord.hpp>
#include <Constant/MapIdentifier.hpp>
#include <Constant/PointOfInterestRecord.hpp>
#include <Constant/RenownHeartRecord.hpp>
#include <Constant/WaypointRecord.hpp>
#include <tuple>

// Content that - although discoverable currently or historically,
// and present in game's dat file is not (easily?)
// assigned to a map.
namespace TWC::WorkaroundUnlistedContent
{
constexpr auto Hearts = {
    std::tuple{ConstantMapIdentifier::WayfarerFoothills, ConstantRenownHeartRecord::AssistGrimarrMolesmasher},
    {ConstantMapIdentifier::HeartOfTheMists, ConstantRenownHeartRecord::CapturePoints},
    {ConstantMapIdentifier::HeartOfTheMists, ConstantRenownHeartRecord::FinishYourEnemies},
    {ConstantMapIdentifier::HeartOfTheMists, ConstantRenownHeartRecord::ReviveAllies},
};
constexpr auto Waypoints = {
    std::tuple{ConstantMapIdentifier::PlainsOfAshford, ConstantWaypointRecord::VaultWaypoint},
    {ConstantMapIdentifier::AscalonianCatacombsStory, ConstantWaypointRecord::CryptWaypoint},
    {ConstantMapIdentifier::KessexHills, ConstantWaypointRecord::MeadowsWaypoint},
    {ConstantMapIdentifier::KessexHills, ConstantWaypointRecord::MoogoolooWaypoint},
    {ConstantMapIdentifier::TheTowerOfNightmaresPublic, ConstantWaypointRecord::TowerOfNightmaresWaypointPublic},
    {ConstantMapIdentifier::TheTowerOfNightmaresPrivate, ConstantWaypointRecord::TowerOfNightmaresWaypointPrivate},
};
constexpr auto PointsOfInterest = {
    std::tuple{ConstantMapIdentifier::LabyrinthineCliffs, ConstantPointOfInterestRecord::PlazaOfPeace},
    {ConstantMapIdentifier::LabyrinthineCliffs, ConstantPointOfInterestRecord::BazaarOfTheFourWinds},
    {ConstantMapIdentifier::HeartOfTheMists, ConstantPointOfInterestRecord::ForestOfAarden},
    {ConstantMapIdentifier::TheRuinedCityOfArah, ConstantPointOfInterestRecord::TemplesGuard},
    {ConstantMapIdentifier::Dragonfall, ConstantPointOfInterestRecord::VerdanceFalls},
};

constexpr auto HeroChallenges = {
    // Krait Obelisk Hero Challenge
    // https://wiki.guildwars2.com/wiki/Krait_Obelisk_Shard_(hero_challenge)
    // is only accessible with an item
    // https://wiki.guildwars2.com/wiki/Krait_Obelisk_Shard_(item)
    // and thus is rather optional, but accessible in each personal instance.
    // Despite that, its not listed in game through regular means and needs to be handled manually.
    // Also, notably - home instance's completion is invisible through world map, and all its
    // content is assigned to corresponding racial capital city.
    /* Racial Capitals */
    std::tuple{ConstantMapIdentifier::HomeInstanceAsura, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceCharr, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceHuman, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceNorn, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceSylvari, ConstantHeroChallengeRecord::KraitObelisk},
    /* Home instances */
    {ConstantMapIdentifier::RacialCapitalAsura, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalCharr, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalHuman, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalNorn, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalSylvari, ConstantHeroChallengeRecord::KraitObelisk},
    /* Homesteads */
    {ConstantMapIdentifier::HomesteadHearthsGlow, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomesteadComosusIsle, ConstantHeroChallengeRecord::KraitObelisk},
};
template <ContentType T> consteval auto Get()
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
    else if constexpr (T == ContentType::HeroChallenge)
    {
        return HeroChallenges;
    }
    else
        return std::initializer_list<std::tuple<int, Record<T>>>{};
}
} // namespace TWC::WorkaroundUnlistedContent
