#pragma once

#include "Options.hpp"
#include "Strings.hpp"
#include "model/Colour.hpp"
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

    std::function<std::tuple<Strings::Identifier, Colour4>(ContentDescriptor)> WorldProgressBar;
    inline Colour4 MapProgressBar(ContentDescriptor dscr)
    {
        return std::get<Colour4>(GetExpansionStyle(dscr));
    }

  private:
    std::tuple<Strings::Identifier, Colour4> GetExpansionStyle(ContentDescriptor);
    static constexpr auto Unknown = TWC::Colour4::rgb(0, 0, 0);
    TWC::Colour4 Core = Unknown;
    TWC::Colour4 HeartOfThorns = Unknown;
    TWC::Colour4 PathOfFire = Unknown;
    TWC::Colour4 EndOfDragons = Unknown;
    TWC::Colour4 SecretsOfTheObscure = Unknown;
    TWC::Colour4 JanthirWilds = Unknown;
    TWC::Colour4 VisionsOfEternity = Unknown;
    ContentDescriptor CoreMask = ContentDescriptor(flags::empty_t{});
};
} // namespace TWC
namespace G
{
extern TWC::StyleManager *Style;
}