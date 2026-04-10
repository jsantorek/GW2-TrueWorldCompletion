#pragma once

#include "Configurable/ContentExclusions.hpp"
#include "Content/Container.hpp"
#include "Content/Type.hpp"
#include "DataAccessor.hpp"
#include "Map/Definition.hpp"
#include "Map/Entity.hpp"
#include "Model/DescribedContent.hpp"
#include <cstdint>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
template <ContentType T> using DescribedContentCollection = std::vector<DescribedContent<T>>;
template <ContentType T> using DescribedContentReferences = std::vector<uint16_t>;
struct ContentCache : public ContentContainer<DescribedContentCollection>
{
    ContentCache(const DataAccessor &);

    static void Validate();

    void Update(const ConfigurableContentExclusions &);

    std::vector<MapDefinition> GetMapDefinitions() const;
    std::vector<uint32_t> GetMapIdentifiers() const;
    bool HasMapContent(uint32_t id) const;

    std::optional<MapEntity<DescribedContentCollection>> GetMapContent(uint32_t id) const;
    std::optional<MapDescriptor> GetMapDescriptor(uint32_t id) const;
    std::optional<MapDefinition> GetMapDefinition(uint32_t id) const;

    bool HasMap(uint32_t id) const;

    inline auto GetStateHash() const
    {
        return Count<ContentType::RenownHeart>() * 31 + Count<ContentType::HeroChallenge>() * 37 +
               Count<ContentType::Waypoint>() * 41 + Count<ContentType::Vista>() * 43 +
               Count<ContentType::PointOfInterest>() * 47;
    }

  private:
    std::vector<MapEntity<DescribedContentReferences>> Maps = {};
    size_t Hash;
    template <ContentType T> void InitializeContent();
};
template <> void ContentCache::InitializeContent<ContentType::Landmark>();
} // namespace TWC

namespace G::Cache
{
extern TWC::ContentCache *Content;
};