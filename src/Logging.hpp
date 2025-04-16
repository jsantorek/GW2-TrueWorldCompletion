#pragma once
#include <Nexus.h>
#include <format>
#include <sstream>

namespace G
{
extern AddonAPI *APIDefs;
}

namespace log
{
template <ELogLevel level> struct StreamLogger
{
    StreamLogger(const std::string_view &funcsig)
    {
        oss << funcsig;
    }

    ~StreamLogger()
    {
        G::APIDefs->Log(level, ADDON_NAME, oss.str().c_str());
    }

    template <typename T> StreamLogger &operator<<(const T &t)
    {
        oss << t;
        return *this;
    }

  private:
    std::stringstream oss;
};
template <ELogLevel L> struct ScopeLogger
{
    template <typename... Args> constexpr ScopeLogger(const std::string_view &str_, Args &&...args) : str(str_)
    {
        if constexpr (sizeof...(Args) == 0)
        {
            G::APIDefs->Log(L, ADDON_NAME, std::format("{} Started", str_).c_str());
        }
        else if constexpr (sizeof...(Args) == 1)
        {
            G::APIDefs->Log(L, ADDON_NAME, std::format("{} Started: {}", str_, std::forward<Args>(args)...).c_str());
        }
        else
        {
            G::APIDefs->Log(L, ADDON_NAME,
                            std::format("{} Started: {}", str_, std::format(std::forward<Args>(args)...)).c_str());
        }
    }

    constexpr ~ScopeLogger()
    {
        G::APIDefs->Log(L, ADDON_NAME, std::format("{} Finished", str).c_str());
    }

    template <typename... Args> [[nodiscard]] static constexpr auto make(const std::string_view &str_, Args &&...args)
    {
        return std::make_shared<ScopeLogger<L>>(str_, std::forward<Args>(args)...);
    }

    const std::string str;
};

consteval auto cropped_funcsig(const std::string_view &funcsig)
{
    constexpr auto prefix = std::string_view{ADDON_ACRONYM "::"};
    const auto end = funcsig.rfind("(");
    if (end == std::string::npos)
        return funcsig;
    auto start = funcsig.rfind(prefix, end - prefix.size());
    if (start == std::string::npos || start + prefix.size() >= end)
        return funcsig;
    start += prefix.size();
    return funcsig.substr(start, end - start);
}
} // namespace log

#define LOG_STREAM(level) log::StreamLogger<ELogLevel_##level>(log::cropped_funcsig(__PRETTY_FUNCTION__))
#define LOG_SCOPE(level, ...)                                                                                          \
    log::ScopeLogger<ELogLevel_##level>::make(log::cropped_funcsig(__PRETTY_FUNCTION__), __VA_ARGS__)
#define LOG(level, ...)                                                                                                \
    G::APIDefs->Log(ELogLevel_##level, ADDON_NAME,                                                                     \
                    std::format("{} {}", log::cropped_funcsig(__PRETTY_FUNCTION__), std::format(__VA_ARGS__)).c_str())
#define LOG_FAST(level, msg) G::APIDefs->Log(ELogLevel_##level, ADDON_NAME, msg)
#define LOG_DEBUG()                                                                                                    \
    G::APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME,                                                                       \
                    std::format("[{}:{}][{}]", __FILE__, __LINE__, log::cropped_funcsig(__PRETTY_FUNCTION__)).c_str())