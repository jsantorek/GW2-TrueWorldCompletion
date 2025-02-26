#include "Globals.hpp"

namespace G
{
AddonAPI *APIDefs = nullptr;
TWC::Options *Options = nullptr;
TWC::Hooks Hooks = {};
namespace Cache
{
TWC::CompletionCache *Completion = nullptr;
TWC::ContentCache *Content = nullptr;
TWC::CharacterCache *Characters = nullptr;
} // namespace Cache
} // namespace G
