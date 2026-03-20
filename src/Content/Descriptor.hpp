#pragma once

#include "Content/Features.hpp"
#include "Map/Descriptor.hpp"

namespace TWC
{
struct ContentDescriptor
{
    flags::flags<ContentFeatures> Features = flags::flags<TWC::ContentFeatures>(flags::empty_t{});
    MapDescriptor Map = {};
};
static_assert(sizeof(ContentDescriptor) <= sizeof(int));
} // namespace TWC
