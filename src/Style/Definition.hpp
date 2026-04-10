#pragma once

#include "Game/Types.h"
#include <optional>
#include <string>
namespace TWC::StyleDefinition
{
struct MapProgress
{
    GW2RE::Colour4 Colour;
};
struct WorldProgress
{
    std::string Text;
    std::string Label;
    GW2RE::Colour4 Colour;
};
struct CharacterProgress
{
    std::string Text;
    std::optional<std::string> Tooltip;
};
} // namespace TWC::StyleDefinition