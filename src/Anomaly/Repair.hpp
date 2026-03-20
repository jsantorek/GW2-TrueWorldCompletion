#pragma once
#include <Constant/HeroChallengeRecord.hpp>
#include <Constant/MapIdentifier.hpp>
#include <Constant/PointOfInterestRecord.hpp>
#include <Constant/RenownHeartRecord.hpp>
#include <Constant/WaypointRecord.hpp>
#include <tuple>

// There are some contents that, although discoverable (either currently or historically)
// are not present in game in a way that would allow association with a map.
// For those to be accounted for in completionc omputation,
// they need to be manually specified and associated with a map.
namespace TWC::AnomalyRepair
{
constexpr auto Maps = {
    ConstantMapIdentifier::RedAlpineBorderlands,
    ConstantMapIdentifier::GreenDesertBorderlands,
    ConstantMapIdentifier::BlueDesertBorderlands,
    ConstantMapIdentifier::RaidOnTheCapricorn,
};

// Retired, historical content
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

// Krait Obelisk Hero Challenge
// https://wiki.guildwars2.com/wiki/Krait_Obelisk_Shard_(hero_challenge)
// is only accessible with an item
// https://wiki.guildwars2.com/wiki/Krait_Obelisk_Shard_(item)
// and thus is rather optional, but accessible in each personal instance.
// Despite that, its not listed in game through regular means and needs to be handled manually.
// Also, notably - home instance's completion is invisible through world map, and all its
// content is assigned to corresponding racial capital city.
constexpr auto HeroChallenges = {
    std::tuple{ConstantMapIdentifier::HomeInstanceAsura, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalAsura, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceCharr, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalCharr, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceHuman, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalHuman, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceNorn, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalNorn, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomeInstanceSylvari, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::RacialCapitalSylvari, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomesteadHearthsGlow, ConstantHeroChallengeRecord::KraitObelisk},
    {ConstantMapIdentifier::HomesteadComosusIsle, ConstantHeroChallengeRecord::KraitObelisk},
};

// Hero Challenges available on Borderlands:
// - Red:
// https://wiki.guildwars2.com/wiki/Defeat_the_Wyvern
// https://wiki.guildwars2.com/wiki/Mysterious_Energy
// https://wiki.guildwars2.com/wiki/Choya_Stew
// - Green
// https://wiki.guildwars2.com/wiki/The_Spider_Queen
// https://wiki.guildwars2.com/wiki/Centaur_Food
// https://wiki.guildwars2.com/wiki/Skritt_Shiny
// - Blue
// https://wiki.guildwars2.com/wiki/Spider_Egg_Clutch
// https://wiki.guildwars2.com/wiki/Centaur_Totem
// https://wiki.guildwars2.com/wiki/Skritt_King_(hero_challenge)
// Are each available on the analogous map of the other variant Desert/Alpine,
// but since half of those maps are retired, they are not listed explicitly and
// need to have their analogues identified manually.
constexpr auto HasBorderlandsAnalogue(auto MapId)
{
    switch (MapId)
    {
    case ConstantMapIdentifier::RedAlpineBorderlands:
        return ConstantMapIdentifier::RedDesertBorderlands;
    case ConstantMapIdentifier::GreenAlpineBorderlands:
        return ConstantMapIdentifier::GreenDesertBorderlands;
    case ConstantMapIdentifier::BlueAlpineBorderlands:
        return ConstantMapIdentifier::BlueDesertBorderlands;
    case ConstantMapIdentifier::RedDesertBorderlands:
        return ConstantMapIdentifier::RedAlpineBorderlands;
    case ConstantMapIdentifier::GreenDesertBorderlands:
        return ConstantMapIdentifier::GreenAlpineBorderlands;
    case ConstantMapIdentifier::BlueDesertBorderlands:
        return ConstantMapIdentifier::BlueAlpineBorderlands;
    default:
        return 0;
    }
}

} // namespace TWC::AnomalyRepair

/* TODO
Map#1098 Claiming Gilded Hollow
*/