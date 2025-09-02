#pragma once
#include "ContentAnalysis.hpp"
#include "ThreadService.h"
#include "model/ContentDescriptor.hpp"
#include "model/DescribedContent.hpp"
#include "model/MapContent.hpp"
#include <future>
#include <set>

namespace TWC
{
class ContentCache
{
  public:
    ContentCache() : Initialized()
    {
    }

    ~ContentCache()
    {
        Initialized.get();
    }

    std::tuple<std::shared_ptr<MapContent>, ContentDescriptor> GetMap(uint32_t id);

    auto GetMaps(ContentDescriptor mask = DescriptorMask::Everything())
    {
        return Maps & mask;
    }

    DiscoverableContent GetContent(ContentDescriptor mask = DescriptorMask::Everything())
    {
        // lock mutex
        return {Tasks & mask, Challanges & mask, Landmarks & mask, Vistas & mask, Waypoints & mask};
    }

    void Update(std::set<uint32_t>, std::set<uint32_t>, std::set<std::string>);

  private:
    void Initialize(std::set<uint32_t> excludedTasks, std::set<uint32_t> excludedPois,
                    std::set<GW2RE::GUID_t> excludedChallanges);
    std::future<void> Initialized;
    DescribedContent<std::shared_ptr<MapContent>> Maps;
    DescribedContent<GW2RE::TaskDef_t *> Tasks;
    DescribedContent<uint32_t> Landmarks, Vistas, Waypoints;
    DescribedContent<GW2RE::GUID_t *> Challanges;
};
} // namespace TWC
namespace G::Cache
{
extern TWC::ContentCache *Content;
}
