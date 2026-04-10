#pragma once

#include "Content/Features.hpp"
#include "Filter/Interface.hpp"

namespace TWC
{
struct FilterUserExclusions : public FilterInterface
{
    [[nodiscard]] bool operator()(ContentDescriptor dscr) const override
    {
        return dscr.Features.test(ContentFeatures::UserExcluded) == 0;
    }
};
} // namespace TWC