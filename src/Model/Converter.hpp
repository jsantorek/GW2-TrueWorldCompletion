#pragma once

#include "Content/Identifier.hpp"
#include "Content/Record.hpp"
#include "DataAccessor.hpp"
#include "Model/Expansion.hpp"
#include <magic_enum/magic_enum_containers.hpp>
#include <type_traits>
#include <unordered_map>

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
    static void Populate(const DataAccessor &);
    static void Clear();
    static std::unique_ptr<std::unordered_map<uint16_t, uint16_t>> ProgressionMapping;
};

template <> struct Converter<ContentType::HeroChallenge>
{
    static Record<ContentType::HeroChallenge> Serialize(const Identifier<ContentType::HeroChallenge> &id);
    static Identifier<ContentType::HeroChallenge> Deserialize(const Record<ContentType::HeroChallenge> &data);
    static magic_enum::containers::array<Expansion, uint16_t> ProgressionMapping;
};

} // namespace TWC