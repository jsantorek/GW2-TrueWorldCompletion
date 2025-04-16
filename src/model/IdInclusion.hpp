#pragma once
#include <cstdint>

namespace TWC
{
struct IdInclusion
{
    uint32_t ID;
    const char *Name;
    bool Active = true;
};
} // namespace TWC