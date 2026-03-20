#pragma once

#include "Content/Container.hpp"
#include "Content/Identifier.hpp"
#include "Content/Type.hpp"
#include "Map/Entity.hpp"
#include "Model/DescribedContent.hpp"
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <set>
#include <unordered_map>

namespace TWC
{
template <ContentType T> using DescribedContentCollection = std::vector<DescribedContent<T>>;
template <ContentType T> using DescribedContentReferences = std::vector<uint16_t>;
struct ContentCache : public ContentContainer<DescribedContentCollection>
{
    // TODO .... ?!
    ContentCache(int) : ContentContainer<DescribedContentCollection>()
    {
    }

    static void Validate();

    std::optional<uint16_t> GetHeartProgressionId(uint32_t) const;

    void Initialize();

    void Update(std::set<Identifier<ContentType::RenownHeart>>, std::set<Identifier<ContentType::HeroChallenge>>,
                std::set<Identifier<ContentType::Landmark>>);

    std::vector<GW2RE::MapDef_t *> GetMapDefinitions() const;
    bool HasMapContent(uint32_t id) const;

    std::optional<MapEntity<DescribedContentCollection>> GetMapContent(uint32_t id) const;
    std::optional<MapDescriptor> GetMapDescriptor(uint32_t id) const;

    bool HasMap(uint32_t id) const;

  private:
    std::unordered_map<uint16_t, uint16_t> HeartProgressionMapping;
    std::vector<MapEntity<DescribedContentReferences>> Maps = {};
    template <ContentType T> void InitializeContent();
};
template <> void ContentCache::InitializeContent<ContentType::Landmark>();
} // namespace TWC

namespace G::Cache
{
extern TWC::ContentCache *Content;
};