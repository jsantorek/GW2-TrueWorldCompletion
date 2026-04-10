#include "Completion/Store.hpp"
#include "Model/Expansion.hpp"
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace
{
auto constexpr Key_MapId = "MapId";
auto constexpr Key_World = "World";
auto constexpr Key_Legendary = "Legendary";
auto constexpr Key_Reward = "Rewards";
} // namespace

void TWC::to_json(nlohmann::json &j, const TWC::LocalizedCompletionStore<uint32_t> &c)
{
    j[Key_MapId] = c.MapId;

    j[Key_World] = {c.World.Completed, c.World.Available};
    j[Key_Legendary] = c.Legendary;
    j[Key_Reward] = c.Reward;
    for (const auto [expansion, name] : magic_enum::enum_entries<Expansion>())
    {
        j[name] = c.Expansions[expansion];
    }
}

void TWC::from_json(const nlohmann::json &j, TWC::LocalizedCompletionStore<uint32_t> &c)
{
    j.at(Key_MapId).get_to(c.MapId);
    if (auto it = j.find(Key_World); it != j.end() && it->is_array())
    {
        c.World.Completed = it.value()[0];
        c.World.Available = it.value()[1];
    }
    if (auto it = j.find(Key_Legendary); it != j.end())
    {
        c.Legendary = it.value();
    }
    if (auto it = j.find(Key_Reward); it != j.end())
    {
        c.Reward = it.value();
    }
    for (const auto [expansion, name] : magic_enum::enum_entries<Expansion>())
    {
        if (auto it = j.find(name); it != j.end())
        {
            c.Expansions[expansion] = it.value();
        }
    }
}
