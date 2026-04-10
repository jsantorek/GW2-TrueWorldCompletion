#pragma once

#include "Completion/Value.hpp"
#include "Model/Expansion.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_containers.hpp>
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
template <typename Value> struct CompletionStore
{
    Value Reward = {};
    Value Legendary = {};
    magic_enum::containers::array<Expansion, Value> Expansions = {};
};
template <typename Value> struct LocalizedCompletionStore : public CompletionStore<Value>
{
    CompletionValue World = {};
    uint32_t MapId = 0;
};
void to_json(nlohmann::json &j, const LocalizedCompletionStore<uint32_t> &c);
void from_json(const nlohmann::json &j, LocalizedCompletionStore<uint32_t> &c);
} // namespace TWC
