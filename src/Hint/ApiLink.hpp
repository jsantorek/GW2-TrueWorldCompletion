#pragma once
#include "Hint/Interface.hpp"
#include <span>
#include <string>

namespace TWC
{
struct HintApiLink : public HintInterface
{
    virtual void SetIncompleteMaps(std::span<MapDefinition>) override;
    virtual void Activate() override;
    virtual bool Available() override;

    std::string Url = {};
};
} // namespace TWC