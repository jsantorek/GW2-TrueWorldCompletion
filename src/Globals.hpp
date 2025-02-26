#pragma once

#include "Options.hpp"
#include "caching/CharacterCache.hpp"
#include "caching/CompletionCache.hpp"
#include "caching/ContentCache.hpp"
#include <Hooks.hpp>
#include <Nexus.h>

namespace G
{
extern AddonAPI *APIDefs;
extern TWC::Options *Options;
extern TWC::Hooks Hooks;
namespace Cache
{
extern TWC::CompletionCache *Completion;
extern TWC::ContentCache *Content;
extern TWC::CharacterCache *Characters;
inline void Delete()
{
    if (G::Cache::Completion)
    {
        delete G::Cache::Completion;
        G::Cache::Completion = nullptr;
    }
    if (G::Cache::Content)
    {
        delete G::Cache::Content;
        G::Cache::Content = nullptr;
    }
    if (G::Cache::Characters)
    {
        delete G::Cache::Characters;
        G::Cache::Characters = nullptr;
    }
}
} // namespace Cache
} // namespace G
