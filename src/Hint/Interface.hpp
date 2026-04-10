#pragma once
#include "Map/Definition.hpp"
#include <span>

namespace TWC
{
struct HintInterface
{
    virtual ~HintInterface() = default;
    virtual void SetIncompleteMaps(std::span<MapDefinition>) = 0;
    virtual void Activate() = 0;
    virtual bool Available() = 0;
};
} // namespace TWC