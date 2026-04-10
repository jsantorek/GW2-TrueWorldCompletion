#pragma once
#include <magic_enum/magic_enum_containers.hpp>

namespace TWC
{
enum class FormatCharacterProgressTextFlags
{
    ShowWorldPercentage = 1 << 0,
    ShowWorldValues = 1 << 1,
    ShowGiftOfBattlePercentage = 1 << 2,
    ShowGiftOfBattleValues = 1 << 3,
};
struct FormatCharacterProgressText : magic_enum::containers::bitset<FormatCharacterProgressTextFlags>
{
    using magic_enum::containers::bitset<FormatCharacterProgressTextFlags>::bitset;
    static constexpr auto Default = {
        FormatCharacterProgressTextFlags::ShowWorldValues,
        FormatCharacterProgressTextFlags::ShowGiftOfBattlePercentage,
    };
};
} // namespace TWC
