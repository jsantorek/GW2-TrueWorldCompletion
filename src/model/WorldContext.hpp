#pragma once

#include "model/MapClassification.hpp"
#include "model/MapContent.hpp"
#include <Common.h>
#include <ContentAnalysis.hpp>
#include <unordered_set>

namespace TWC
{
struct MapContent;
class WorldContext
{
  public:
    WorldContext(std::unordered_set<std::shared_ptr<MapContent>> all, std::unordered_set<uint32_t> excluded)
        : VisitedMaps(std::move(excluded)),
          Context(ContentAnalysis::Classify(GW2RE::Core::GetPropContext().GetMissionCtx()->CurrentMap)),
          AllMaps(std::move(all))
    {
    }
    WorldContext(const WorldContext &) = delete;
    virtual ~WorldContext() = default;

    WorldContext &operator++();

    WorldContext &begin();

    std::unordered_set<std::shared_ptr<MapContent>>::const_iterator end() const
    {
        return AllMaps.end();
    }
    MapContent *operator*() const
    {
        return CurrentMap->get();
    }

    virtual bool IsIncluded(MapClassification) const = 0;

    friend bool operator!=(const WorldContext &a,
                           const std::unordered_set<std::shared_ptr<MapContent>>::const_iterator &b)
    {
        return a.CurrentMap != b;
    }

  protected:
    const MapClassification Context;

  private:
    std::unordered_set<uint32_t> VisitedMaps;
    std::unordered_set<std::shared_ptr<MapContent>>::const_iterator CurrentMap;
    const std::unordered_set<std::shared_ptr<MapContent>> AllMaps;
};

class WholeWorld : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification) const override
    {
        return true;
    }
};
class SameContinentOnly : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification map) const override
    {
        return map.Continent == Context.Continent;
    }
};
class TheMistsOrSynchronousExpansion : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification map) const override
    {
        if (Context.Continent == MapClassification::TheMists)
            return map.Continent == MapClassification::TheMists;
        return map.Chronology == Context.Chronology;
    }
};
class TheMistsOrAccessibleExpansion : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification map) const override
    {
        if (Context.Continent == MapClassification::TheMists)
            return map.Continent == MapClassification::TheMists;
        return map.Expansion == Context.Expansion;
    }
};
class ExpansionsReleasedByCurrent : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification map) const override
    {
        return map.Chronology <= Context.Chronology;
    }
};
class ExpansionsAccessibleByCurrent : public WorldContext
{
  public:
    using WorldContext::WorldContext;
    bool IsIncluded(MapClassification map) const override
    {
        return map.Expansion <= Context.Expansion;
    }
};
} // namespace TWC
