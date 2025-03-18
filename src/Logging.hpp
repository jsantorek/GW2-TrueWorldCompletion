#pragma once
#include <Nexus.h>
#include <sstream>

template <ELogLevel level> struct ScopedLogging
{
    ScopedLogging(LOGGER_LOG2 log, char const *func) : _log(log)
    {
        _ss << func;
    }

    std::stringstream &stream()
    {
        return _ss;
    }

    ~ScopedLogging()
    {
        if (_log)
            _log(level, ADDON_NAME, _ss.str().c_str());
    }

  private:
    std::stringstream _ss;
    LOGGER_LOG2 _log;
};

#include "Globals.hpp"
#define LOG_TRACE() ScopedLogging<ELogLevel_TRACE>(G::APIDefs->Log, __func__).stream()
#define LOG_DEBUG() ScopedLogging<ELogLevel_DEBUG>(G::APIDefs->Log, __func__).stream()
#define LOG_CRITICAL() ScopedLogging<ELogLevel_CRITICAL>(G::APIDefs->Log, __func__).stream()
#define LOG_INFO() ScopedLogging<ELogLevel_INFO>(G::APIDefs->Log, __func__).stream()
#define LOG_FAST(level, msg) G::APIDefs->Log(ELogLevel_##level, ADDON_NAME, msg)
