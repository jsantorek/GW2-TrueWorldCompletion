#pragma once
#include "Configurable/ExclusionReason.hpp"
#include "Content/Container.hpp"
#include "Content/Identifier.hpp"
#include "Content/Record.hpp"
#include "Filter/Interface.hpp"
#include "Model/Converter.hpp"
#include <magic_enum/magic_enum_containers.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <unordered_set>

namespace TWC
{
template <ContentType T> using ContentRecordSet = std::unordered_set<Record<T>>;
template <ContentType T> using ContentRecordVector = std::vector<Record<T>>;
struct ConfigurableContentExclusions : public ContentContainer<ContentRecordSet>
{
    static ConfigurableContentExclusions Default();
    void Render();
    template <ContentType T> inline auto GetIdentifiers() const
    {
        auto ids = std::unordered_set<Identifier<T>>{};
        for (const auto &record : GetContents<T>())
        {
            ids.emplace(Converter<T>::Deserialize(record));
        }
        return ids;
    }
    std::unique_ptr<FilterInterface> GetFilter() const;
    size_t GetStateHash() const;
    magic_enum::containers::set<ConfigurableExclusionReason> AutoExclusions;

  private:
    static magic_enum::containers::array<ConfigurableExclusionReason, std::string_view> MakeExclusionNames();
    std::optional<ContentContainer<ContentRecordVector>> ExclusionCandidates = std::nullopt;
};
void to_json(nlohmann::json &, const ConfigurableContentExclusions &);
void from_json(const nlohmann::json &, ConfigurableContentExclusions &);
} // namespace TWC