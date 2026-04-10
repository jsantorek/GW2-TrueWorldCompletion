#pragma once

#include "Constant/MapIdentifier.hpp"
#include "Model/Expansion.hpp"
#include <optional>
#include <tuple>

namespace TWC::WorkaroundAccessAnomaly
{
// These maps were at some point available to all players so accessibility restrictions are inconsistent
constexpr std::optional<std::tuple<Expansion, bool>> ForMap(uint32_t id)
{
    switch (id)
    {
    case ConstantMapIdentifier::BastionOfThePenitentLabel:
        return std::tuple{Expansion::HeartOfThorns, false};
    case ConstantMapIdentifier::CrystalOasis:
    case ConstantMapIdentifier::SparkingTheFlame:
        return std::tuple{Expansion::PathOfFire, false};
    case ConstantMapIdentifier::MountBalriorLabel:
        return std::tuple{Expansion::JanthirWilds, false};
    case ConstantMapIdentifier::HomesteadHearthsGlow:
        return std::tuple{Expansion::JanthirWilds, true};
    case ConstantMapIdentifier::HomesteadComosusIsle:
        return std::tuple{Expansion::VisionsOfEternity, true};

    default:
        break;
    }
    return std::nullopt;
}
} // namespace TWC::WorkaroundAccessAnomaly
