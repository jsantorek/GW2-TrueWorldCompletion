#pragma once
#include "Game/Content/EContentType.h"
#include "Game/MapLayout/MapDetailsMap.h"
#include "Game/MapLayout/MapLayoutMap.h"
#include "Game/PointOfInterest/PointOfInterestDef.h"
#include "Game/Task/TaskSubRegionCommonDef.h"
#include "Game/Types.h"
#include <Nexus.h>
#include <vector>

namespace TWC
{
struct DataAccessor
{
    DataAccessor();
    template <typename T> std::vector<T *> GetAll() const;
    template <typename T> T *Get(const GW2RE::GUID_t &) const;

  private:
    GW2RE::MapLayoutMap_t *GetReference() const;
    GW2RE::EContentType MapLayoutMap;
    GW2RE::EContentType TaskSubRegionCommonDef;
    GW2RE::EContentType PointOfInterestDef;
};
template <> std::vector<GW2RE::MapLayoutMap_t *> DataAccessor::GetAll<GW2RE::MapLayoutMap_t>() const;
template <> std::vector<GW2RE::PointOfInterestDef_t *> DataAccessor::GetAll<GW2RE::PointOfInterestDef_t>() const;
template <>
std::vector<GW2RE::TaskSubRegionCommonDef_t *> DataAccessor::GetAll<GW2RE::TaskSubRegionCommonDef_t>() const;
template <> GW2RE::MapDetailsMap_t *DataAccessor::Get<GW2RE::MapDetailsMap_t>(const GW2RE::GUID_t &) const;

} // namespace TWC
