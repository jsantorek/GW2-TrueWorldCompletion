#pragma once

#include "model/CachedCompletion.hpp"
#include <Logging.hpp>
#include <Options.hpp>
#include <nlohmann/json.hpp>

namespace flags
{
template <class T> void to_json(nlohmann::json &j, const flags<T> &c)
{
    for (const auto &flag : magic_enum::enum_values<T>())
        if (c.count(flag))
            j.emplace_back(magic_enum::enum_name(flag));
}
template <class T> void from_json(const nlohmann::json &j, flags<T> &c)
{
    c.clear();
    for (auto flag : j.get<std::vector<std::string>>())
    {
        if (auto parsed = magic_enum::enum_cast<T>(flag))
            c.emplace(parsed.value());
        else
            LOG(WARNING, "Could not parse {} as one of {} flags!", flag, magic_enum::enum_type_name<T>());
    }
}
} // namespace flags
namespace TWC
{
inline void to_json(nlohmann::json &j, const Options::ProgressBarColours &c)
{
    const auto reference = Options::ProgressBarColours();
    j = nlohmann::json::object_t();
    for (const auto &[feature, colour] : reference)
    {
        if (c.count(feature) && c.at(feature) != colour)
        {
            if (feature == DescriptorMask::Everything())
                j.emplace("COMBINED", c.at(feature));
            else
                j.emplace(magic_enum::enum_name(feature), c.at(feature));
        }
    }
}
inline void from_json(const nlohmann::json &j, Options::ProgressBarColours &c)
{
    c = Options::ProgressBarColours();
    for (const auto &[id, colour] : j.get<std::map<std::string, std::array<float, 4>>>())
    {
        if (auto feature = magic_enum::enum_cast<ContentFeature>(id))
        {
            if (c.count(feature.value()))
                c[feature.value()] = colour;
            else
                LOG(WARNING, "Feature {} isn't used to identify progress bar colour", id);
        }
        else if (id.starts_with("COMBINED"))
        {
            constexpr auto combined = static_cast<ContentFeature>(DescriptorMask::Everything().underlying_value());
            c[combined] = colour;
        }
        else
            LOG(WARNING, "Could not parse {} as one of {} flags!", id, magic_enum::enum_type_name<ContentFeature>());
    }
}
inline void to_json(nlohmann::json &j, const Options::ContentExclusion &c)
{
    j["Landmarks"] = c.Landmarks;
    j["SkillChallanges"] = c.SkillChallanges;
    j["Tasks"] = c.Tasks;
}

inline void from_json(const nlohmann::json &j, Options::ContentExclusion &c)
{
    j.at("Landmarks").get_to(c.Landmarks);
    j.at("SkillChallanges").get_to(c.SkillChallanges);
    j.at("Tasks").get_to(c.Tasks);
}

template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
void to_json(nlohmann::json &j, const Enum &e)
{
    if (auto name = magic_enum::enum_name(e); !name.empty())
        j = std::string{name};
    else
        j = static_cast<std::underlying_type_t<Enum>>(e);
}

template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
void from_json(const nlohmann::json &j, Enum &e)
{
    if (j.is_string())
    {
        if (auto opt = magic_enum::enum_cast<Enum>(j.get<std::string>()))
        {
            e = opt.value();
            return;
        }
    }
    else if (j.is_number_integer())
    {
        if (auto opt = magic_enum::enum_cast<Enum>(j.get<std::underlying_type_t<Enum>>()))
        {
            e = opt.value();
            return;
        }
    }
    LOG(WARNING, "Could not parse {} as {}", j.dump(), magic_enum::enum_type_name<Enum>());
}

inline void to_json(nlohmann::json &j, const CachedCompletion &c)
{
    j["MapId"] = c.MapId;
    j["Available"] = c.Progress.Available;
    j["Completed"] = c.Progress.Completed;
}
inline void from_json(const nlohmann::json &j, CachedCompletion &c)
{
    j.at("MapId").get_to(c.MapId);
    j.at("Available").get_to(c.Progress.Available);
    j.at("Completed").get_to(c.Progress.Completed);
}
} // namespace TWC