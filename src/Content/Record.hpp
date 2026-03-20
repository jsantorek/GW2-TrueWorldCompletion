#pragma once

#include "Content/Type.hpp"
#include <cstdint>
#include <string>

namespace TWC
{
template <ContentType T> struct RecordInfo
{
    using type = uint32_t;
};

template <> struct RecordInfo<ContentType::HeroChallenge>
{
    using type = std::string;
};

template <ContentType T> using Record = RecordInfo<T>::type;

} // namespace TWC