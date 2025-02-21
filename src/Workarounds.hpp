#pragma once

#include "model/CustomCompletion.hpp"
#include <optional>

namespace TWC::Workarounds
{
/*
There are some maps that are not trivial to handle and
contain some inconsistencies in discoverable content
but are fine if treated as completely different map.
This function maps ids of such maps.
*/
std::optional<uint32_t> GetMapIdReplacement(uint32_t id);

/*
There are some maps with incosistencies severe enough
that substitution for another id will not fix them.
This function provides mechanism for custom completion computation.
*/
std::optional<CustomCompletion> GetCustomCompletion(uint32_t id);

/*
To verify if map contains only content of its own
and thus should be accounted for in world completion, its explicitly checked.
Some maps though should not have its content validted, eg. they were handled in
GetMapIdReplacement or GetCustomCompletion
*/
bool ShouldSkipContentValidation(uint32_t id);

} // namespace TWC::Workarounds
