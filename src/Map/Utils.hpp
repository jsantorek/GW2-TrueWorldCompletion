#pragma once

#include "Configurable/ExpansionAssignment.hpp"
#include "Model/Continent.hpp"
#include "Model/Expansion.hpp"
#include <optional>

namespace TWC::MapUtils
{
std::optional<Expansion> MatchExpansions(ConfigurableExpansionAssignment, uint32_t, uint32_t);
std::optional<Continent> MatchContinent(uint32_t, uint32_t);
} // namespace TWC::MapUtils