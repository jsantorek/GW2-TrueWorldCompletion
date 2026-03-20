#pragma once
#include <Content/Record.hpp>

namespace TWC::ConstantRenownHeartRecord
{
// https://wiki.guildwars2.com/wiki/Assist_Grimarr_Molesmasher
constexpr auto AssistGrimarrMolesmasher = Record<ContentType::RenownHeart>{58};
// https://wiki.guildwars2.com/wiki/Capture_Points!
constexpr auto CapturePoints = Record<ContentType::RenownHeart>{81};
// https://wiki.guildwars2.com/wiki/Finish_Your_Enemies!
constexpr auto FinishYourEnemies = Record<ContentType::RenownHeart>{82};
// https://wiki.guildwars2.com/wiki/Revive_Allies!
constexpr auto ReviveAllies = Record<ContentType::RenownHeart>{83};
} // namespace TWC::ConstantRenownHeartRecord
