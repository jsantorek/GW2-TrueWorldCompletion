#include <array>
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <options/ColoursConfigurator.hpp>

TWC::ColourConfigurator::ColourConfigurator(std::unordered_map<ContentFeature, std::array<float, 4>> &colours)
    : Colours(colours), NamedDescriptors({
                            {"Core", ContentFeature::EXPANSION_None},
                            {"Heart of Thorns", ContentFeature::EXPANSION_HeartOfThorns},
                            {"Path of Fire", ContentFeature::EXPANSION_PathOfFire},
                            {"End of Dragons", ContentFeature::EXPANSION_EndOfDragons},
                            {"Secrets of the Obscure", ContentFeature::EXPANSION_SecretsOfTheObscure},
                            {"Janthir Wilds", ContentFeature::EXPANSION_JanthirWilds},
                            {"Visions of Eternity", ContentFeature::EXPANSION_VisionsOfEternity},
                            {"The Mists", ContentFeature::CONTINENT_TheMists},
                            {"Multiple combined", static_cast<ContentFeature>(DescriptorMask::Everything().underlying_value())},
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
