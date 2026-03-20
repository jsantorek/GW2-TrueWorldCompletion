#pragma once

#include "Completion/Value.hpp"
#include "Content/Type.hpp"

namespace TWC
{
struct CompletionInfo
{
    CompletionValue PointsOfInterest;
    CompletionValue Challenges;
    CompletionValue Hearts;
    CompletionValue Waypoints;
    CompletionValue Vistas;
    CompletionValue Total;

    CompletionInfo &operator+=(const CompletionInfo &other)
    {
        this->PointsOfInterest += other.PointsOfInterest;
        this->Challenges += other.Challenges;
        this->Hearts += other.Hearts;
        this->Waypoints += other.Waypoints;
        this->Vistas += other.Vistas;
        this->Total += other.Total;
        return *this;
    }

    inline void RefreshTotal()
    {
        this->Total = this->PointsOfInterest + this->Challenges + this->Hearts + this->Waypoints + this->Vistas;
    }

    template <ContentType T> inline auto &GetValue()
    {
        if constexpr (T == ContentType::RenownHeart)
        {
            return Hearts;
        }
        else if constexpr (T == ContentType::HeroChallenge)
        {
            return Challenges;
        }
        else if constexpr (T == ContentType::Waypoint)
        {
            return Waypoints;
        }
        else if constexpr (T == ContentType::Vista)
        {
            return Vistas;
        }
        else if constexpr (T == ContentType::PointOfInterest)
        {
            return PointsOfInterest;
        }
        else
        {
            static_assert(T != T, "GetContents not implemented for this ContentType");
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const CompletionInfo &mc)
    {
        os << "  PointsOfInterest: " << mc.PointsOfInterest << "\n"
           << "  Challenges:       " << mc.Challenges << "\n"
           << "  Hearts:           " << mc.Hearts << "\n"
           << "  Waypoints:        " << mc.Waypoints << "\n"
           << "  Vistas:           " << mc.Vistas << "\n"
           << "  Total:            " << mc.Total;
        return os;
    }
};
} // namespace TWC
