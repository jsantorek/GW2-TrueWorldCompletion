#include <array>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <options/ColoursConfigurator.hpp>

using namespace TWC::Retired;
TWC::ColourConfigurator::ColourConfigurator(std::unordered_map<Retired::ContentFeature, std::array<float, 4>> &colours)
    : Colours(colours),
      NamedDescriptors({
          {"Core", Retired::ContentFeature::EXPANSION_None},
          {"Heart of Thorns", Retired::ContentFeature::EXPANSION_HeartOfThorns},
          {"Path of Fire", Retired::ContentFeature::EXPANSION_PathOfFire},
          {"End of Dragons", Retired::ContentFeature::EXPANSION_EndOfDragons},
          {"Secrets of the Obscure", Retired::ContentFeature::EXPANSION_SecretsOfTheObscure},
          {"Janthir Wilds", Retired::ContentFeature::EXPANSION_JanthirWilds},
          {"Visions of Eternity", Retired::ContentFeature::EXPANSION_VisionsOfEternity},
          {"The Mists", Retired::ContentFeature::CONTINENT_TheMists},
          {"Multiple combined", static_cast<Retired::ContentFeature>(DescriptorMask::Everything().underlying_value())},
      })
{
}

void TWC::ColourConfigurator::Render()
{
    for (auto &[descr, id] : NamedDescriptors)
    {
        auto it = Colours.find(id);
        if (it == Colours.end())
            ImGui::TextColored({1, 0, 0, 1}, "Failed to identify colour for [\"%s\" %d]", descr, id);
        else
            ImGui::ColorEdit4(descr, it->second.data());
    }
}
