#include "model/Converter.hpp"
#include "Constant/Undisclosed.hpp"
#include "Content/Cache.hpp"
#include "Content/Identifier.hpp"
#include <charconv>
#include <cstddef>
#include <format>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <string>

namespace TWC
{

Record<ContentType::RenownHeart> Converter<ContentType::RenownHeart>::Serialize(
    const Identifier<ContentType::RenownHeart> &id)
{
    return static_cast<Record<ContentType::RenownHeart>>(id.ID);
}

Identifier<ContentType::RenownHeart> Converter<ContentType::RenownHeart>::Deserialize(
    const Record<ContentType::RenownHeart> &data)
{
    using IdType = decltype(std::declval<Identifier<ContentType::RenownHeart>>().ID);
    if (data > (std::numeric_limits<IdType>::max)())
        return {};
    if (auto progress = G::Cache::Content->GetHeartProgressionId(data))
        return Identifier<ContentType::RenownHeart>{
            .ID = static_cast<IdType>(data),
            .ProgressID = *progress,
        };
    return {};
}

Record<ContentType::HeroChallenge> Converter<ContentType::HeroChallenge>::Serialize(
    const Identifier<ContentType::HeroChallenge> &id)
{
    constexpr auto Mapping = ConstantUndisclosed::HeroChallengeProgressionMapping;
    auto it = std::find(Mapping.begin(), Mapping.end(), id.ProgressID);
    if (it == Mapping.end())
        return {};
    return std::format("{}-{}", std::distance(Mapping.begin(), it), id.Bit);
}

Identifier<ContentType::HeroChallenge> Converter<ContentType::HeroChallenge>::Deserialize(
    const Record<ContentType::HeroChallenge> &data)
{
    using BitType = decltype(std::declval<Identifier<ContentType::HeroChallenge>>().Bit);
    unsigned long exp = 0, bit = 0;
    size_t pos = data.find('-');
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

} // namespace TWC
