#pragma once
#include "model/Colour.hpp"

namespace TWC
{
static constexpr auto WorldMapProgressWidgetWidthMultiplier = 1.2f;
static constexpr auto ConfigFilename = "config.json";
static constexpr auto CacheFilename = "cache.json";
static constexpr auto HttpsMaxMapIdCount = 200;
static constexpr auto CacheVersion = 1;
static constexpr auto ConfigVersion = 1;
static constexpr auto CharacterSelectionCompletionFormat = L"[{}/{}] {:.1f}%";
static constexpr auto WorldCompletionProgressBarFormat = L"{:.1f}%";
namespace Colours
{
static constexpr auto Core = Colour4::rgb(222, 41, 9);
static constexpr auto HeartOfThorns = Colour4::rgb(15, 138, 37);
static constexpr auto PathOfFire = Colour4::rgb(102, 19, 81);
static constexpr auto EndOfDragons = Colour4::rgb(72, 201, 136);
static constexpr auto SecretsOfTheObscure = Colour4::rgb(230, 184, 13);
static constexpr auto JanthirWilds = Colour4::rgb(40, 80, 159);
static constexpr auto TheMists = Colour4::rgb(132, 129, 126);
static constexpr auto Unknown = Colour4::rgb(0, 0, 0);
static constexpr auto Default = Colour4::rgb(75, 181, 211);
static constexpr auto Legendary = Colour4::rgb(153, 127, 37);
static constexpr auto Text = Colour4::rgb(255, 222, 181);
} // namespace Colours
} // namespace TWC
