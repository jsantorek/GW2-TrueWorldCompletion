#pragma once

#include "Game/Types.h"
#include <string>
namespace TWC::StyleDefinition
{
struct MapProgress
{
    GW2RE::Colour4 Colour;
};
struct WorldProgress
{
    std::wstring Text;
    std::wstring Label;
    GW2RE::Colour4 Colour;
};
struct CharacterProgress
{
    std::wstring Text;
    std::wstring Tooltip;
};
} // namespace TWC::StyleDefinition