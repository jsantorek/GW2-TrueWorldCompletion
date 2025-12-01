#pragma once

#include "Game/Types.h"
#include "Options.hpp"
#include "Strings.hpp"
#include "model/ContentDescriptor.hpp"
#include <flags/flags.hpp>
#include <tuple>

namespace TWC
{
class StyleManager
{
  public:
    StyleManager();
    void Update(const std::unordered_map<ContentFeature, std::array<float, 4>> &, Options::ExpansionAssignmentMode,
                Options::CompletionMode);

    std::function<std::tuple<Strings::Identifier, GW2RE::Colour4>(ContentDescriptor)> WorldProgressBar;
    inline GW2RE::Colour4 MapProgressBar(ContentDescriptor dscr)
    {
        return std::get<GW2RE::Colour4>(GetExpansionStyle(dscr));
    }

  private:
    std::tuple<Strings::Identifier, GW2RE::Colour4> GetExpansionStyle(ContentDescriptor);
    static constexpr auto Unknown = GW2RE::Colour4(0, 0, 0);
    GW2RE::Colour4 Core = Unknown;
    GW2RE::Colour4 HeartOfThorns = Unknown;
    GW2RE::Colour4 PathOfFire = Unknown;
    GW2RE::Colour4 EndOfDragons = Unknown;
    GW2RE::Colour4 SecretsOfTheObscure = Unknown;
    GW2RE::Colour4 JanthirWilds = Unknown;
    GW2RE::Colour4 VisionsOfEternity = Unknown;
    ContentDescriptor CoreMask = ContentDescriptor(flags::empty_t{});
};
} // namespace TWC
namespace G
{
extern TWC::StyleManager *Style;
}