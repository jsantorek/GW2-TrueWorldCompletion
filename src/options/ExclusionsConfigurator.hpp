#pragma once
#include "ContentAnalysis.hpp"
#include "Exclusions.hpp"
#include "model/ContentDescriptor.hpp"
#include <model/Content.hpp>
#include <set>
#include <vector>
namespace TWC
{
class ExclusionsConfigurator
{
  public:
    ExclusionsConfigurator(std::set<ContentInfo<ContentType::RenownHeart>::SerializationType> &, std::set<uint32_t> &,
                           std::set<ContentInfo<ContentType::HeroChallenge>::SerializationType> &);
    void Render();
    void DrawMultiToggles();
    void DrawMapDetails();
    void DrawMapAddition();
    void DrawIgnoreButtons();

  private:
    void AddUnlistedExclusions();
    void AddExplicitExclusions();
    void ExcludeContent(ContentFeature);
    std::vector<MapExclusion> Maps;
    std::set<ContentInfo<ContentType::RenownHeart>::SerializationType> &Tasks;
    std::set<uint32_t> &Pois;
    std::set<ContentInfo<ContentType::HeroChallenge>::SerializationType> &Challanges;
    ContentAnalysis::SkillChallengeIDConverter Converter;
};
} // namespace TWC
