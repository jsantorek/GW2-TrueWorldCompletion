#include "Factory.hpp"
#include "Configurable/ExpansionAssignment.hpp"
#include "Configurable/WorldCompletion.hpp"
#include "Content/Descriptor.hpp"
#include "Filter/CompletionReward.hpp"
#include "Filter/Continent.hpp"
#include "Filter/Expansion.hpp"
#include "Filter/Interface.hpp"
#include "Filter/UserExclusions.hpp"
#include "Logging.hpp"
#include "Map/Descriptor.hpp"
#include "Model/Expansion.hpp"
#include <cstddef>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>
#include <mutex>
#include <utility>

namespace
{
struct SkipEverything : public TWC::FilterInterface
{
    [[nodiscard]] bool operator()(TWC::ContentDescriptor) const override
    {
        return false;
    }
};
template <typename T> inline auto MakeLocationBasedFilter() -> std::unique_ptr<TWC::FilterInterface>
{
    auto descriptor = TWC::MapDescriptor::FromCurrentMap();
    if (descriptor)
        return std::make_unique<T>(*descriptor);
    return std::make_unique<SkipEverything>();
}
template <TWC::ConfigurableExpansionAssignment A, std::size_t... I>
inline auto MakeExpansionFilters(std::index_sequence<I...>)
{
    return magic_enum::containers::array<TWC::Expansion, std::unique_ptr<TWC::FilterInterface>>{
        std::make_unique<TWC::FilterExpansion<A>>(static_cast<TWC::Expansion>(I))...};
}
template <TWC::ConfigurableExpansionAssignment A>
inline auto MakeWorldFilter(TWC::ConfigurableWorldCompletion completion) -> std::unique_ptr<TWC::FilterInterface>
{
    switch (completion)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received, falling back to default {}",
            magic_enum::enum_type_name<TWC::ConfigurableWorldCompletion>(), magic_enum::enum_name(completion),
            magic_enum::enum_name(TWC::ConfigurableWorldCompletion::AllMapsCollectively));
    case TWC::ConfigurableWorldCompletion::AllMapsCollectively:
        return std::make_unique<TWC::FilterUserExclusions>();
    case TWC::ConfigurableWorldCompletion::AllMapsWithCompletionReward:
        return std::make_unique<TWC::FilterCompletionReward>();
    case TWC::ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps:
        return ::MakeLocationBasedFilter<TWC::FilterSubsequentExpansion<A>>();
    case TWC::ConfigurableWorldCompletion::CurrentContinentMapsOnly:
        return ::MakeLocationBasedFilter<TWC::FilterContinent>();
    case TWC::ConfigurableWorldCompletion::CurrentExpansionMapsOnly:
        return ::MakeLocationBasedFilter<TWC::FilterExpansion<A>>();
    }
}
} // namespace

std::unique_ptr<TWC::FilterInterface> TWC::FilterFactory::MakeWorldFilter()
{
    const auto lock = std::lock_guard(Mutex);
    switch (Assignment)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received, falling back to default {}",
            magic_enum::enum_type_name<ConfigurableExpansionAssignment>(), magic_enum::enum_name(Assignment),
            magic_enum::enum_name(ConfigurableExpansionAssignment::AccessabilityBased));
    case ConfigurableExpansionAssignment::AccessabilityBased:
        return ::MakeWorldFilter<ConfigurableExpansionAssignment::AccessabilityBased>(Completion);
    case ConfigurableExpansionAssignment::ChronologyBased:
        return ::MakeWorldFilter<ConfigurableExpansionAssignment::ChronologyBased>(Completion);
    }
}

void TWC::FilterFactory::Update(ConfigurableExpansionAssignment assignment, ConfigurableWorldCompletion completion)
{
    const auto lock = std::lock_guard(Mutex);
    Assignment = assignment;
    Completion = completion;
    Hash = (static_cast<size_t>(Assignment) << 32) | static_cast<size_t>(Completion);
}
magic_enum::containers::array<TWC::Expansion, std::unique_ptr<TWC::FilterInterface>> TWC::FilterFactory::
    MakeExpansionFilters()
{
    const auto lock = std::lock_guard(Mutex);
    constexpr auto indices = std::make_index_sequence<magic_enum::enum_count<TWC::Expansion>()>{};
    switch (Assignment)
    {
    default:
        LOG(WARNING, "Unsupported {}::{} received, falling back to default {}",
            magic_enum::enum_type_name<ConfigurableExpansionAssignment>(), magic_enum::enum_name(Assignment),
            magic_enum::enum_name(ConfigurableExpansionAssignment::AccessabilityBased));
    case ConfigurableExpansionAssignment::AccessabilityBased:
        return ::MakeExpansionFilters<ConfigurableExpansionAssignment::AccessabilityBased>(indices);
    case ConfigurableExpansionAssignment::ChronologyBased:
        return ::MakeExpansionFilters<ConfigurableExpansionAssignment::ChronologyBased>(indices);
    }
}
