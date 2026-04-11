#pragma once
#include "Configurable/ExpansionAssignment.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Filter/Interface.hpp"
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>
#include <mutex>

namespace TWC
{
struct FilterFactory
{
    void Update(ConfigurableExpansionAssignment, ConfigurableWorldCompletion);
    std::unique_ptr<FilterInterface> MakeWorldFilter();
    magic_enum::containers::array<Expansion, std::unique_ptr<FilterInterface>> MakeExpansionFilters();
    inline auto GetStateHash() const
    {
        return Hash;
    }
    std::mutex Mutex;
    ConfigurableExpansionAssignment Assignment = ConfigurableExpansionAssignment::ChronologyBased;
    ConfigurableWorldCompletion Completion = ConfigurableWorldCompletion::AllMapsCollectively;
    size_t Hash;
};
} // namespace TWC
namespace G
{
extern std::unique_ptr<TWC::FilterFactory> Filters;
};