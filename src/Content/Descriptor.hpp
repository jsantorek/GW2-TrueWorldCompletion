#pragma once

#include "Content/Features.hpp"
#include "Map/Descriptor.hpp"

namespace TWC
{
struct ContentDescriptor
{
    magic_enum::containers::bitset<ContentFeatures> Features{magic_enum::containers::detail::raw_access_t{}};
    MapDescriptor Map = {};
};
static_assert(sizeof(ContentDescriptor) <= sizeof(int));
} // namespace TWC
