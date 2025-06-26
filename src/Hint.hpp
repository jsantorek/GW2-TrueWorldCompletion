#pragma once
#include "Map.h"
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

class MapCinematicHint : public Hint
{
  public:
    ~MapCinematicHint() = default;

    void Activate() override;
};

class MapRecenterHint : public Hint
{
  public:
    ~MapRecenterHint() = default;
    void Activate() override;
    void Update(std::vector<const GW2RE::MapDef_t *> ids) override;

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
    void Update(std::vector<const GW2RE::MapDef_t *> ids) override
    {
        Hint::Update(std::move(ids));
        last = incomplete.begin();
    }

  private:
    std::vector<const GW2RE::MapDef_t *>::iterator last;
};
} // namespace TWC
