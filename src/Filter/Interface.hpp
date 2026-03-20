#pragma once
#include "Content/Descriptor.hpp"

namespace TWC
{
struct FilterInterface
{
    [[nodiscard]] virtual bool operator()(ContentDescriptor) const = 0;
    virtual ~FilterInterface() = default;
};
} // namespace TWC