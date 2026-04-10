#include "Configurable/IncompleteMapHint.hpp"
#include "Configurable/MinorPatch.hpp"
#include "ExclusionReason.hpp"
#include "ExpansionAssignment.hpp"
#include "WorldCompletion.hpp"
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <utility>

void TWC::to_json(nlohmann::json &j, const TWC::ConfigurableExclusionReason &c)
{
    j = magic_enum::enum_name(c);
}

void TWC::from_json(const nlohmann::json &j, TWC::ConfigurableExclusionReason &c)
{
    auto name = j.get<std::string>();
    if (auto casted = magic_enum::enum_cast<ConfigurableExclusionReason>(name))
    {
        c = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
}

void TWC::to_json(nlohmann::json &j, const TWC::ConfigurableIncompleteMapHint &c)
{
    j = magic_enum::enum_name(c);
}

void TWC::from_json(const nlohmann::json &j, TWC::ConfigurableIncompleteMapHint &c)
{
    auto name = j.get<std::string>();
    if (auto casted = magic_enum::enum_cast<ConfigurableIncompleteMapHint>(name))
    {
        c = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
}

void TWC::to_json(nlohmann::json &j, const TWC::ConfigurableExpansionAssignment &c)
{
    j = magic_enum::enum_name(c);
}

void TWC::from_json(const nlohmann::json &j, TWC::ConfigurableExpansionAssignment &c)
{
    auto name = j.get<std::string>();
    if (auto casted = magic_enum::enum_cast<ConfigurableExpansionAssignment>(name))
    {
        c = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
}

void TWC::to_json(nlohmann::json &j, const TWC::ConfigurableWorldCompletion &c)
{
    j = magic_enum::enum_name(c);
}

void TWC::from_json(const nlohmann::json &j, TWC::ConfigurableWorldCompletion &c)
{
    auto name = j.get<std::string>();
    if (auto casted = magic_enum::enum_cast<ConfigurableWorldCompletion>(name))
    {
        c = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
}

void TWC::to_json(nlohmann::json &j, const TWC::ConfigurableMinorPatch &c)
{
    j = magic_enum::enum_name(c);
}

void TWC::from_json(const nlohmann::json &j, TWC::ConfigurableMinorPatch &c)
{
    auto name = j.get<std::string>();
    if (auto casted = magic_enum::enum_cast<ConfigurableMinorPatch>(name))
    {
        c = casted.value();
    }
    else
    {
        throw std::runtime_error(std::move(name));
    }
}
