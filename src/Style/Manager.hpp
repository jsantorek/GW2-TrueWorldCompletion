#pragma once
#include "Completion/Byte.hpp"
#include "Completion/Store.hpp"
#include "Completion/Value.hpp"
#include "Game/Types.h"
#include "Map/Descriptor.hpp"
#include "Model/Expansion.hpp"
#include "Style/Definition.hpp"

namespace TWC
{
class StyleManager
{
  public:
    StyleDefinition::MapProgress GetMapProgress(TWC::MapDescriptor) const;
    StyleDefinition::WorldProgress GetWorldProgress(CompletionValue) const;
    StyleDefinition::CharacterProgress GetCharacterProgress(std::wstring_view, uint32_t, CompletionByte) const;

  private:
    GW2RE::Colour4 GetExpansionColour(TWC::Expansion) const;
    std::tuple<GW2RE::Colour4, std::wstring> GetWorldColourAndName(uint32_t) const;
    std::wstring SerializeGiftOfExplorationProgress(CompletionByte) const;
    std::wstring SerializeCompletionProgress(const LocalizedCompletionStore<CompletionValue> &) const;
};
} // namespace TWC
namespace G
{
extern TWC::StyleManager *Style;
}