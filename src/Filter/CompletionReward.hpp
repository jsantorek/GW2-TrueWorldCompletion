#pragma once

#include "Content/Features.hpp"
#include "Filter/Interface.hpp"
#include "Map/Features.hpp"

namespace TWC
{
struct FilterCompletionReward : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        return dscr.Map.Features.test(MapFeatures::HasCompletionReward) &&
               dscr.Features.test(ContentFeatures::Retired) == 0;
    }
};
} // namespace TWC