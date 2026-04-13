#include "Converter.hpp"
#include "Constant/Undisclosed.hpp"
#include "Content/Identifier.hpp"
#include "Content/Record.hpp"
#include "Content/Type.hpp"
#include "DataAccessor.hpp"
#include "Game/Task/TaskSubRegionCommonDef.h"
#include "Model/Expansion.hpp"
#include <algorithm>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iterator>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <string>
#include <system_error>
#include <unordered_map>
#include <utility>

std::unordered_map<uint16_t, uint16_t> *TWC::Converter<TWC::ContentType::RenownHeart>::ProgressionMapping = nullptr;

TWC::Record<TWC::ContentType::RenownHeart> TWC::Converter<TWC::ContentType::RenownHeart>::Serialize(
    const Identifier<ContentType::RenownHeart> &id)
{
    return static_cast<Record<ContentType::RenownHeart>>(id.ID);
}

void TWC::Converter<TWC::ContentType::RenownHeart>::Populate(const DataAccessor &data)
{
    if (ProgressionMapping)
        ProgressionMapping->clear();
    else
        ProgressionMapping = new std::unordered_map<uint16_t, uint16_t>{};
    for (auto task : data.GetAll<GW2RE::TaskSubRegionCommonDef_t>())
    {
        ProgressionMapping->emplace(static_cast<uint16_t>(task->ID),
                                    task->Progress ? static_cast<uint16_t>(task->Progress->ID) : 0);
    }
}

void TWC::Converter<TWC::ContentType::RenownHeart>::Clear()
{
    delete ProgressionMapping;
    ProgressionMapping = nullptr;
}

TWC::Identifier<TWC::ContentType::RenownHeart> TWC::Converter<TWC::ContentType::RenownHeart>::Deserialize(
    const Record<ContentType::RenownHeart> &data)
{
    if (data > (std::numeric_limits<uint16_t>::max)())
        return {};
    const auto id = static_cast<uint16_t>(data);
    if (const auto progress = ProgressionMapping->find(id); progress != ProgressionMapping->end())
        return Identifier<ContentType::RenownHeart>{
            .ID = id,
            .ProgressID = progress->second,
        };
    return {};
}

TWC::Record<TWC::ContentType::HeroChallenge> TWC::Converter<TWC::ContentType::HeroChallenge>::Serialize(
    const Identifier<ContentType::HeroChallenge> &id)
{
    constexpr auto Mapping = ConstantUndisclosed::HeroChallengeProgressionMapping;
    auto it = std::find(Mapping.begin(), Mapping.end(), id.ProgressID);
    if (it == Mapping.end())
        return {};
    return std::format("{}-{}", std::distance(Mapping.begin(), it), id.Bit);
}

TWC::Identifier<TWC::ContentType::HeroChallenge> TWC::Converter<TWC::ContentType::HeroChallenge>::Deserialize(
    const Record<ContentType::HeroChallenge> &data)
{
    using BitType = decltype(std::declval<Identifier<ContentType::HeroChallenge>>().Bit);
    unsigned long exp = 0, bit = 0;
    const size_t pos = data.find('-');
    if (pos == std::string::npos)
        return {};
    auto res = std::from_chars(data.data(), data.data() + pos, exp);
    if (res.ec != std::errc())
        return {};
    res = std::from_chars(data.data() + pos + 1, data.data() + data.size(), bit);
    if (res.ec != std::errc())
        return {};
    if (exp >= magic_enum::enum_count<Expansion>() || bit > (std::numeric_limits<BitType>::max)())
        return {};
    return {
        .ProgressID = *(ConstantUndisclosed::HeroChallengeProgressionMapping.begin() + exp),
        .Bit = static_cast<BitType>(bit),
    };
}
