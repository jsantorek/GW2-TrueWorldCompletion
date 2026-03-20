#pragma once
#include "Constant/MapIdentifier.hpp"
#include "Model/Expansion.hpp"
#include "Model/Festival.hpp"

namespace TWC::AnomalyReplace
{

auto constexpr ForsakenThicketMaps = {
    ConstantMapIdentifier::SpiritVale,
    ConstantMapIdentifier::SalvationPass,
    ConstantMapIdentifier::StrongholdOfTheFaithful,
};
auto constexpr BastionOfThePenitentMaps = {1188};

constexpr auto Maps = {
    std::tuple{ConstantMapIdentifier::ForsakenThicket, ForsakenThicketMaps},
    {ConstantMapIdentifier::BastionOfThePenitentClone, BastionOfThePenitentMaps},
};

constexpr auto FestivalMaps = {
    std::tuple{ConstantMapIdentifier::ToypocalypsePublic, Festival::Wintersday},
    {ConstantMapIdentifier::SnowballMayhem, Festival::Wintersday},
    {ConstantMapIdentifier::TixxsInfinirarium, Festival::Wintersday},
    {ConstantMapIdentifier::ReapersRumble, Festival::Halloween},
    {ConstantMapIdentifier::AscentToMadness, Festival::Halloween},
    {ConstantMapIdentifier::MadKingsLabyrinth, Festival::Halloween},
    {ConstantMapIdentifier::MadKingsRaceway, Festival::Halloween},
    {ConstantMapIdentifier::DragonBallArena, Festival::LunarNewYear},
    {ConstantMapIdentifier::AspectArena, Festival::FestivalOfTheFourWinds},
    {ConstantMapIdentifier::LabyrinthineCliffs, Festival::FestivalOfTheFourWinds},
    {ConstantMapIdentifier::TheCrownPavilion, Festival::FestivalOfTheFourWinds},
};
constexpr auto ExpansionMaps = {std::tuple{ConstantMapIdentifier::HomesteadComosusIsle, Expansion::VisionsOfEternity},
                                {ConstantMapIdentifier::HomesteadHearthsGlow, Expansion::JanthirWilds}};
} // namespace TWC::AnomalyReplace
