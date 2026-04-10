#pragma once
#include "Game/Map/MapDef.h"
#include "Hint/Interface.hpp"
#include "Tools/ThreadService.h"
#include <span>
#include <vector>

namespace TWC
{
struct HintMapRecenter : public HintInterface
{
    HintMapRecenter();
    virtual void SetIncompleteMaps(std::span<MapDefinition>) override;
    virtual void Activate() override;
    virtual bool Available() override;

    std::vector<GW2RE::MapDef_t *> Incomplete;
    std::vector<GW2RE::MapDef_t *>::const_iterator Next;
    std::shared_ptr<ThreadService> Thread;
};
} // namespace TWC