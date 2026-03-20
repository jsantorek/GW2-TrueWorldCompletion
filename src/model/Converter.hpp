#pragma once

#include "Content/Identifier.hpp"
#include "Content/Record.hpp"
#include <type_traits>

namespace TWC
{

template <ContentType T> struct Converter
{
    static Record<T> Serialize(const Identifier<T> &id)
    {
        if constexpr (T == ContentType::PointOfInterest || T == ContentType::Waypoint || T == ContentType::Vista ||
                      T == ContentType::Landmark)
        {
            return Record<T>{id.ID};
        }
        else
            static_assert(T != T, "No Converter::Serialize for this ContentType");
    }

    static Identifier<T> Deserialize(const Record<T> &data)
    {
        if constexpr (T == ContentType::PointOfInterest || T == ContentType::Waypoint || T == ContentType::Vista ||
                      T == ContentType::Landmark)
        {
            return Identifier<T>{data};
        }
        else
            static_assert(std::false_type::value, "No Converter::Deserialize for this ContentType");
    }
};

template <> struct Converter<ContentType::RenownHeart>
{
    static Record<ContentType::RenownHeart> Serialize(const Identifier<ContentType::RenownHeart> &id);
    static Identifier<ContentType::RenownHeart> Deserialize(const Record<ContentType::RenownHeart> &data);
};

template <> struct Converter<ContentType::HeroChallenge>
{
    static Record<ContentType::HeroChallenge> Serialize(const Identifier<ContentType::HeroChallenge> &id);
    static Identifier<ContentType::HeroChallenge> Deserialize(const Record<ContentType::HeroChallenge> &data);
};

} // namespace TWC