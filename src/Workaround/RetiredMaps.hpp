#pragma once
#include <Constant/HeroChallengeRecord.hpp>
#include <Constant/MapIdentifier.hpp>
#include <Constant/PointOfInterestRecord.hpp>
#include <Constant/RenownHeartRecord.hpp>
#include <Constant/WaypointRecord.hpp>

namespace TWC::WorkaroundRetiredMaps
{
constexpr auto Identifiers = {
    ConstantMapIdentifier::RedAlpineBorderlands,     ConstantMapIdentifier::GreenDesertBorderlands,
    ConstantMapIdentifier::BlueDesertBorderlands,    ConstantMapIdentifier::RaidOnTheCapricorn,
    ConstantMapIdentifier::ResealingTheBloodyPrince, ConstantMapIdentifier::TheReliquary,
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
constexpr auto inline HeroChallengesMovedFrom(auto MapId)
{
    switch (MapId)
    {
    case ConstantMapIdentifier::RedAlpineBorderlands:
        return ConstantMapIdentifier::RedDesertBorderlands;
    case ConstantMapIdentifier::GreenDesertBorderlands:
        return ConstantMapIdentifier::GreenAlpineBorderlands;
    case ConstantMapIdentifier::BlueDesertBorderlands:
        return ConstantMapIdentifier::BlueAlpineBorderlands;
    default:
        return 0;
    }
}
} // namespace TWC::WorkaroundRetiredMaps