#pragma once

#include "Model/Expansion.hpp"
#include <string>

namespace TWC::TextLocalization
{
void Initialize();
std::wstring FetchDescription(Expansion);
constexpr auto Unknown = L"???";
constexpr auto Uncached = L" -- ";
} // namespace TWC::TextLocalization
