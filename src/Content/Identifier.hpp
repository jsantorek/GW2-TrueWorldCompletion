#pragma once

#include "Content/Type.hpp"
#include <compare>
#include <cstdint>

namespace TWC
{

struct LandmarkId
{
    uint32_t ID = {};

    LandmarkId() = default;
    explicit LandmarkId(uint32_t id) : ID(id)
    {
    }

    explicit operator bool() const
    {
        return ID != 0;
    }

    std::strong_ordering operator<=>(const LandmarkId &other) const noexcept
    {
        return ID <=> other.ID;
    }

    bool operator==(const LandmarkId &other) const noexcept
    {
        return ID == other.ID;
    }
    bool operator!=(const LandmarkId &other) const noexcept
    {
        return ID != other.ID;
    }
};

struct WaypointId : public LandmarkId
{
    using LandmarkId::LandmarkId;
};
struct VistaId : public LandmarkId
{
    using LandmarkId::LandmarkId;
};
struct PointOfInterestId : public LandmarkId
{
    using LandmarkId::LandmarkId;
};

struct HeroChallengeId
{
    uint16_t ProgressID = {};
    uint16_t Bit = {};

    explicit operator bool() const
    {
        return ProgressID != 0;
    }

    bool operator==(const HeroChallengeId &other) const noexcept
    {
        return Bit == other.Bit && ProgressID == other.ProgressID;
    }
    bool operator!=(const HeroChallengeId &other) const noexcept
    {
        return Bit != other.Bit || ProgressID != other.ProgressID;
    }

    std::strong_ordering operator<=>(const HeroChallengeId &other) const noexcept
    {
        return ProgressID == other.ProgressID ? (Bit <=> other.Bit) : (ProgressID <=> other.ProgressID);
    }
};

struct RenownHeartId
{
    uint16_t ID = {};
    uint16_t ProgressID = {};

    explicit operator bool() const
    {
        return ID != 0;
    }

    std::strong_ordering operator<=>(const RenownHeartId &other) const noexcept
    {
        return ID <=> other.ID;
    }

    bool operator==(const RenownHeartId &other) const noexcept
    {
        return ID == other.ID;
    }
    bool operator!=(const RenownHeartId &other) const noexcept
    {
        return ID != other.ID;
    }
};

template <ContentType> struct IdentifierOf;

template <> struct IdentifierOf<ContentType::Landmark>
{
    using type = LandmarkId;
};
template <> struct IdentifierOf<ContentType::Waypoint>
{
    using type = WaypointId;
};
template <> struct IdentifierOf<ContentType::Vista>
{
    using type = VistaId;
};
template <> struct IdentifierOf<ContentType::PointOfInterest>
{
    using type = PointOfInterestId;
};
template <> struct IdentifierOf<ContentType::HeroChallenge>
{
    using type = HeroChallengeId;
};
template <> struct IdentifierOf<ContentType::RenownHeart>
{
    using type = RenownHeartId;
};

template <ContentType T> using Identifier = typename IdentifierOf<T>::type;

} // namespace TWC