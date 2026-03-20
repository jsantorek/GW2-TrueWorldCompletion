#pragma once
#include <model/ContentDescriptor.hpp>
#include <unordered_map>
#include <vector>

namespace TWC
{
class ColourConfigurator
{
  public:
    ColourConfigurator(std::unordered_map<Retired::ContentFeature, std::array<float, 4>> &);
    void Render();

  private:
    std::unordered_map<Retired::ContentFeature, std::array<float, 4>> &Colours;
    const std::vector<std::tuple<const char *, Retired::ContentFeature>> NamedDescriptors;
};
} // namespace TWC
