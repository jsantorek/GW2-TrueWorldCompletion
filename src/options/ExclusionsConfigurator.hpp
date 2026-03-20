#pragma once
#include "Exclusions.hpp"
#include <Content/Record.hpp>
#include <set>
#include <vector>
namespace TWC
{
class ExclusionsConfigurator
{
  public:
    ExclusionsConfigurator(std::set<Record<ContentType::RenownHeart>> &, std::set<Record<ContentType::Landmark>> &,
                           std::set<Record<ContentType::HeroChallenge>> &);
    void Render();
    void DrawMultiToggles();
    void DrawMapDetails();
    void DrawMapAddition();
    void DrawIgnoreButtons();

  private:
    void AddUnlistedExclusions();
    void AddExplicitExclusions();
    std::vector<MapExclusion> Maps;
    std::set<Record<ContentType::RenownHeart>> &Tasks;
    std::set<Record<ContentType::Landmark>> &Pois;
    std::set<Record<ContentType::HeroChallenge>> &Challanges;
};
} // namespace TWC
