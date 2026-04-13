#pragma once

#include "Model/Expansion.hpp"
#include <Nexus.h>
#include <string>

namespace TWC::TextLocalization
{
void Initialize(const AddonAPI::LocalizationVT &localization);
std::wstring FetchDescription(Expansion);
constexpr auto Unknown = L"???";
constexpr auto Uncached = L" -- ";
} // namespace TWC::TextLocalization
