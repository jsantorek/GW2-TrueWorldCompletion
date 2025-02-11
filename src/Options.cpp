#include "Options.hpp"
#include <Globals.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>

namespace TWC
{
void to_json(nlohmann::json &j, const IdInclusion &i)
{
    j = nlohmann::json{{"id", i.ID}, {"name", i.Name}, {"active", i.Active}};
}

void from_json(const nlohmann::json &j, IdInclusion &i)
{
    i = IdInclusion{j.at("id"), j["name"].get<std::string>().c_str(), j.at("active")};
}
void Options::Persist(std::filesystem::path file) const
{
    try
    {
        auto json = nlohmann::json();
        json["WorldCompletion"] = WorldCompletion;
        std::set<int> ExcludedMaps;
        for (const auto &[key, inclusions] : MapInclusions)
        {
            for (const auto &inclusion : inclusions)
            {
                if (!inclusion.Active)
                    ExcludedMaps.insert(inclusion.ID);
            }
        }
        json["ExcludedMaps"] = ExcludedMaps;
        std::set<int> ExcludedContinents;
        for (const auto &[_, inclusion] : ContinentsAndExpansionsInclusions)
        {
            if (inclusion.ID == ContinentsAndExpansionsEnumeration::Undetermined ||
                inclusion.ID == ContinentsAndExpansionsEnumeration::ToBeDetermined)
                continue;
            if (!inclusion.Active)
                ExcludedContinents.insert(inclusion.ID);
        }
        json["ExcludedContinents"] = ExcludedContinents;
        if (!std::filesystem::exists(file.parent_path()))
        {
            std::filesystem::create_directories(file.parent_path());
        }
        std::ofstream(file) << json;
    }
    catch (const std::exception &e)
    {
        G::APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, e.what());
        G::APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, "Unexpected error when persisting options");
    }
}

void Options::Parse(std::filesystem::path file)
{
    try
    {
        auto json = nlohmann::json::object();
        if (std::filesystem::exists(file))
        {
            json = nlohmann::json::parse(std::ifstream(file), nullptr, false);
        }
        WorldCompletion = json.value("WorldCompletion", WorldCompletion);
        const auto ExcludedMaps = json["ExcludedMaps"].get<std::set<int>>();
        for (auto &[key, inclusions] : MapInclusions)
        {
            for (auto &inclusion : inclusions)
            {
                if (ExcludedMaps.count(inclusion.ID))
                    inclusion.Active = false;
            }
        }
        const auto ExcludedContinents = json["ExcludedContinents"].get<std::set<int>>();
        for (auto &[_, inclusion] : ContinentsAndExpansionsInclusions)
        {
            if (inclusion.ID == ContinentsAndExpansionsEnumeration::Undetermined ||
                inclusion.ID == ContinentsAndExpansionsEnumeration::ToBeDetermined)
                continue;
            if (ExcludedContinents.count(inclusion.ID))
                inclusion.Active = false;
        }
    }
    catch (const std::exception &e)
    {
        G::APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, e.what());
        G::APIDefs->Log(ELogLevel_WARNING, ADDON_NAME,
                        "Unexpected error when parsing options, falling back to defaults");
    }
}
} // namespace TWC