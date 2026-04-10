#pragma once
#include "Hint/Interface.hpp"
#include <span>
#include <vector>

namespace TWC
{
struct HintWikiLink : public HintInterface
{
    virtual void SetIncompleteMaps(std::span<MapDefinition>) override;
    virtual void Activate() override;
    virtual bool Available() override;

    std::vector<uint32_t> Incomplete = {};
    std::vector<uint32_t>::const_iterator Next;
};
} // namespace TWC