#pragma once

#include "Options.hpp"
#include <Hooks.hpp>
#include <Nexus.h>
#include <windows.h>

namespace G
{
extern HMODULE Module;
extern AddonAPI *APIDefs;
extern TWC::Options Options;
extern TWC::Hooks Hooks;
} // namespace G
