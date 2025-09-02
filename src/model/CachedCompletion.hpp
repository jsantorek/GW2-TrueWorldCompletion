#pragma once
#include "model/Completion.hpp"

namespace TWC
{
struct CachedCompletion
{
    Completion Progress;
    uint32_t MapId;
};
} // namespace TWC
