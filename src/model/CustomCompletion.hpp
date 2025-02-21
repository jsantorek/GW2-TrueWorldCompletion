#pragma once

#include <set>

namespace TWC
{
struct CustomCompletion
{
    std::set<uint32_t> BaseMaps;
    std::set<uint32_t> PoiExclusions;
};
} // namespace TWC
