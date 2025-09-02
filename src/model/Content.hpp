#pragma once
#include <string>

namespace TWC
{
enum class ContentType
{
    Vista,
    PointOfInterest,
    Waypoint,
    RenownHeart,
    HeroChallenge,
    Landmark
};
template <ContentType T> struct ContentInfo
{
    using SerializationType = uint32_t;
};
template <> struct ContentInfo<ContentType::HeroChallenge>
{
    using SerializationType = std::string;
};
} // namespace TWC