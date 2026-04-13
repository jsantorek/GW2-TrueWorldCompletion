#pragma once

#include "Configurable/ColourPalette.hpp"
#include "Configurable/ContentExclusions.hpp"
#include "Configurable/ExpansionAssignment.hpp"
#include "Configurable/IncompleteMapHint.hpp"
#include "Configurable/MinorPatch.hpp"
#include "Configurable/TextFormat.hpp"
#include "Configurable/WorldCompletion.hpp"
#include <Nexus.h>
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>

namespace TWC
{
class Options
{
  public:
    void Apply() const;
    void Save() const;
    void Reload();
    static std::unique_ptr<Options> Load();

    static void SetupConfiguration(const AddonAPI::RendererVT &renderer, const AddonAPI::PathsVT &paths);
    static void CleanupConfiguration();

  private:
    static void RenderConfiguration();
    static void RenderUtilityButtons();
    static magic_enum::containers::array<ConfigurableExpansionAssignment, std::string_view> MakeAssignmentNames();
    static magic_enum::containers::array<ConfigurableWorldCompletion, std::string_view> MakeCompletionNames();
    static magic_enum::containers::array<ConfigurableIncompleteMapHint, std::string_view> MakeHintNames();
    void RenderGeneral();
    ConfigurableMinorPatches MinorPatches = ConfigurableMinorPatches::Default;
    ConfigurableIncompleteMapHint IncompleteMapHint = ConfigurableIncompleteMapHint::Default;
    ConfigurableContentExclusions ContentExclusions = ConfigurableContentExclusions::Default();
    ConfigurableColourPalette ColourPalette = ConfigurableColourPalette::Default();
    ConfigurableWorldCompletion WorldCompletion = ConfigurableWorldCompletion::Default;
    ConfigurableExpansionAssignment ExpansionAssignment = ConfigurableExpansionAssignment::Default;
    ConfigurableTextFormat TextFormat = ConfigurableTextFormat::Default();
};
} // namespace TWC
