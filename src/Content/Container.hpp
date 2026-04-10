#pragma once

#include "Content/Type.hpp"
#include <utility>

namespace TWC
{
template <template <ContentType> typename Collection> struct ContentContainer
{
    ContentContainer() = default;

    template <ContentType T> inline const auto &GetContents() const
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

    template <ContentType T> inline auto &GetContents()
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

    template <ContentType T> inline auto HasContents() const
    {
        return GetContents<T>().size() != 0;
    }

    template <ContentType T> inline auto Count() const
    {
        return GetContents<T>().size();
    }

    inline auto Empty() const
    {
        return GetContents<ContentType::Vista>().empty() && GetContents<ContentType::PointOfInterest>().empty() &&
               GetContents<ContentType::Waypoint>().empty() && GetContents<ContentType::RenownHeart>().empty() &&
               GetContents<ContentType::HeroChallenge>().empty();
    }

    template <ContentType T, typename UnaryFunc, typename UnaryPred>
    void ApplyIf(const UnaryPred &pred, const UnaryFunc &func) const
    {
        for (const auto &c : GetContents<T>())
            if (pred(c))
                func(c);
    }

    template <ContentType T, typename UnaryFunc> void Apply(const UnaryFunc &func) const
    {
        for (const auto &c : GetContents<T>())
            func(c);
    }

    template <typename UnaryFunc> void Apply(const UnaryFunc &func) const
    {
        for (const auto &c : GetContents<ContentType::HeroChallenge>())
            func(c);
        for (const auto &c : GetContents<ContentType::RenownHeart>())
            func(c);
        for (const auto &c : GetContents<ContentType::PointOfInterest>())
            func(c);
        for (const auto &c : GetContents<ContentType::Vista>())
            func(c);
        for (const auto &c : GetContents<ContentType::Waypoint>())
            func(c);
    }

    template <ContentType T, typename UnaryPred> bool AnyOf(const UnaryPred &pred) const
    {
        for (const auto &c : GetContents<T>())
        {
            if (pred(c))
                return true;
        }
        return false;
    }

    template <ContentType T, typename UnaryPred> bool NoneOf(const UnaryPred &pred) const
    {
        for (const auto &c : GetContents<T>())
        {
            if (pred(c))
                return false;
        }
        return true;
    }
    template <ContentType T, typename UnaryPred> bool AllOf(const UnaryPred &pred) const
    {
        for (const auto &c : GetContents<T>())
        {
            if (!pred(c))
                return false;
        }
        return true;
    }
    template <ContentType T, class Init, typename BinaryOp> Init Accumulate(Init init, const BinaryOp &op) const
    {
        for (const auto &c : GetContents<T>())
        {
            init = op(std::move(init), c);
        }
        return init;
    }

    Collection<ContentType::RenownHeart> Hearts = {};
    Collection<ContentType::HeroChallenge> Challenges = {};
    Collection<ContentType::PointOfInterest> PointsOfInterest = {};
    Collection<ContentType::Waypoint> Waypoints = {};
    Collection<ContentType::Vista> Vistas = {};
};
} // namespace TWC
