#pragma once

#include "model/Content.hpp"
#include <algorithm>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <set>
namespace TWC
{
enum class ExclusionReason : uint32_t
{
    Custom = 0,
    Inaccessible,
    Historical,
    Exclusive_Gemstore,
    Exclusive_Story,
    Seasonal_Wintersday,
    Seasonal_Halloween,
    Seasonal_LunarNewYear,
    Seasonal_FestivalOfTheFourWinds,
    Seasonal_SuperAdventureBox,
    Count
};
struct IContentExclusion
{
    virtual ~IContentExclusion() = default;
    virtual void SetEnabled(bool) const = 0;
    virtual bool IsEnabled() const = 0;
    virtual std::size_t Hash() const = 0;
    IContentExclusion(std::string description, ExclusionReason reason)
        : Description(std::move(description)), Reason(reason)
    {
    }
    std::string Description;
    ExclusionReason Reason;
    template <typename T> static std::size_t Hash(const T id, const std::set<T> &container)
    {
        return std::hash<T>()(id) ^ std::hash<const void *>()(&container);
    }
};
template <ContentType T> struct ContentExclusion : IContentExclusion
{
    using Identifier = ContentInfo<T>::SerializationType;
    ContentExclusion(Identifier id, std::set<Identifier> &container, ExclusionReason reason = ExclusionReason::Custom)
        : ID(id), Container(container), IContentExclusion(std::format("{} #{}", magic_enum::enum_name(T), id), reason)
    {
    }
    void SetEnabled(bool include) const override
    {
        if (include)
            Container.emplace(ID);
        else
            Container.erase(ID);
    }
    bool IsEnabled() const override
    {
        return Container.contains(ID);
    }
    std::size_t Hash() const override
    {
        return IContentExclusion::Hash(ID, Container);
    }

    Identifier ID;
    std::set<Identifier> &Container;
};
struct MapExclusion
{
    std::string Description;
    std::vector<std::unique_ptr<IContentExclusion>> Exclusions;
    uint32_t ID;
    template <ContentType T, typename... Args> MapExclusion &With(Args &&...args)
    {
        Exclusions.emplace_back(std::make_unique<ContentExclusion<T>>(std::forward<Args>(args)...));
        return *this;
    }
    template <ContentType T, typename... Args> void WithExcluded(Args &&...args)
    {
        Exclusions.emplace_back(std::make_unique<ContentExclusion<T>>(std::forward<Args>(args)...))->SetEnabled(true);
    }
    MapExclusion(uint32_t id, const std::string &name = {})
        : Description(name.empty() ? std::format("Map#{}", id) : std::format("Map#{} {}", id, name)), ID(id)
    {
    }
    template <typename T> bool Contains(T id, const std::set<T> &container) const
    {
        auto hash = IContentExclusion::Hash(id, container);
        return std::any_of(Exclusions.begin(), Exclusions.end(),
                           [hash](const auto &exclusion) { return exclusion->Hash() == hash; });
    }

    MapExclusion(const std::string &name = {}) : Description(name.empty() ? "Other" : name), ID(0)
    {
    }
};
}; // namespace TWC