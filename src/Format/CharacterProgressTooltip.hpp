#pragma once
#include <magic_enum/magic_enum_containers.hpp>

namespace TWC
{
enum class FormatCharacterProgressTooltipFlags
{
    ShowPercentages = 1 << 0,
    ShowValues = 1 << 1,
};
struct FormatCharacterProgressTooltip : magic_enum::containers::bitset<FormatCharacterProgressTooltipFlags>
{
    using magic_enum::containers::bitset<FormatCharacterProgressTooltipFlags>::bitset;
    static constexpr auto Default = {FormatCharacterProgressTooltipFlags::ShowPercentages,
                                     FormatCharacterProgressTooltipFlags::ShowValues};
    [[nodiscard]] auto inline IsCustomized() const
    {
        return any();
    }
};
} // namespace TWC
