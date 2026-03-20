#pragma once

#include "Model/Expansion.hpp"
#include <array>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json_fwd.hpp>

namespace TWC
{
template <typename Value> struct CompletionStore
{
    Value World = {};
    Value Reward = {};
    std::array<Value, magic_enum::enum_count<Expansion>()> Expansions = {};
};
template <typename Value> struct LocalizedCompletionStore : public CompletionStore<Value>
{
    uint32_t MapId;
};
void to_json(nlohmann::json &j, const LocalizedCompletionStore<uint32_t> &c);
void from_json(const nlohmann::json &j, LocalizedCompletionStore<uint32_t> &c);
} // namespace TWC
