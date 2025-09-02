#pragma once
#include "Game/Map.h"
#include <vector>
namespace TWC
{
class Hint
{
  public:
    Hint() = default;
    virtual ~Hint() = default;
    virtual void Activate() = 0;

    virtual void Update(std::vector<const GW2RE::MapDef_t *> maps)
    {
        incomplete = std::move(maps);
    }

  protected:
    std::vector<const GW2RE::MapDef_t *> incomplete;
};

class MapRecenterHint : public Hint
{
  public:
    ~MapRecenterHint() = default;
    void Activate() override;
    void Update(std::vector<const GW2RE::MapDef_t *> maps) override;

  private:
    void ActivateMapRecenter(const GW2RE::MapDef_t *map);
    std::vector<const GW2RE::MapDef_t *>::iterator last;
};

class LinkAPIHint : public Hint
{
  public:
    ~LinkAPIHint() = default;

    void Activate() override;
};

class LinkWikiHint : public Hint
{
  public:
    ~LinkWikiHint() = default;
    void Activate() override;
    void Update(std::vector<const GW2RE::MapDef_t *> maps) override
    {
        Hint::Update(std::move(maps));
        last = incomplete.begin();
    }

  private:
    std::vector<const GW2RE::MapDef_t *>::iterator last;
};
} // namespace TWC
