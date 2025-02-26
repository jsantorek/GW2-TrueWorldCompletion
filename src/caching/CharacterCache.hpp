#pragma once

#include "model/Completion.hpp"
#include <guiddef.h>
#include <unordered_map>

namespace TWC
{
class CharacterCache
{
    std::unordered_map<GUID, Completion> WorldCompletion;
};
} // namespace TWC
