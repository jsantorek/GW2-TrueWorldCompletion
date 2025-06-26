#pragma once
#include <cstdint>

namespace TWC
{
enum class MapCategory : uint8_t
{
    GemstoreExclusive,
    StoryExclusive,
    Historical,
    Seasonal_Wintersday,
    Seasonal_MadKingsRealm,
    Seasonal_LunarNewYear,
    Seasonal_FestivalOfTheFourWinds,
    Seasonal_SuperAdventureBox,
    Count
};
struct MapInclusion
{
    MapCategory Category;
    uint32_t ID;
    const char *Name;
};

} // namespace TWC