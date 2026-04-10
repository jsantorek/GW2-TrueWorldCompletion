#pragma once

#include "Constant/MapIdentifier.hpp"
#include <initializer_list>
#include <tuple>

namespace TWC::WorkaroundCoupledMaps
{
auto constexpr ForsakenThicketMaps = {
    ConstantMapIdentifier::SpiritVale,
    ConstantMapIdentifier::SalvationPass,
    ConstantMapIdentifier::StrongholdOfTheFaithful,
};
auto constexpr MountBalriorMaps = {
    ConstantMapIdentifier::MountBalriorConvergencePrivate,
    ConstantMapIdentifier::MountBalriorRaid,
    ConstantMapIdentifier::MountBalriorConvergencePublic,
    ConstantMapIdentifier::MountBalriorStoryInstance,
};
constexpr auto WorldMapLabels = {
    std::tuple{ConstantMapIdentifier::ForsakenThicket, ForsakenThicketMaps},
    {ConstantMapIdentifier::MountBalriorLabel, MountBalriorMaps},
};
} // namespace TWC::WorkaroundCoupledMaps
