#pragma once
#include <cstdint>

namespace TWC
{
enum class Continent : uint8_t
{
    None,
    Tyria,
    Mists,
};
static_assert(static_cast<int>(Continent::Tyria) == 1, "Continent::Tyria numerical value does not match API");
static_assert(static_cast<int>(Continent::Mists) == 2, "Continent::Mists numerical value does not match API");
} // namespace TWC