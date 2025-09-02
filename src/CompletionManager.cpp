#include "CompletionManager.hpp"
#include "ContentAnalysis.hpp"
#include "Logging.hpp"
#include "model/ContentDescriptor.hpp"
#include <flags/flags.hpp>

std::string to_string(TWC::ContentDescriptor dscr)
{
    std::string res = "";
    for (const auto &flag : magic_enum::enum_values<TWC::ContentFeature>())
        if (dscr.count(flag))
            res += std::string(" ").append(magic_enum::enum_name(flag));
    return res.empty() ? res : res.substr(1);
}
TWC::CompletionManager::CompletionManager ::CompletionManager()
    : WorldCompletionMask([]() { return ContentDescriptor(flags::empty_t{}); }),
      CharacterSelectionMask([]() { return ContentDescriptor(flags::empty_t{}); })
{
}

std::function<TWC::ContentDescriptor(void)> TWC::CompletionManager::MakeMask(TWC::Options::CompletionMode mode)
{
    switch (mode)
    {
    case Options::AllMapsCollectively:
        return []() { return DescriptorMask::Everything(); };
    case Options::CurrentOrEarlierExpansionMaps:
        return [core = CoreMask]() {
            auto dscr = TWC::ContentAnalysis::ClassifyCurrentMap();
            if (dscr & core)
                return ContentDescriptor(ContentFeature::EXPANSION_None);
            dscr &= DescriptorMask::Expansions();
            return DescriptorMask::ThisOrEarlierExpansion(dscr);
        };
    case Options::CurrentContinentMapsOnly:
        return []() {
            auto dscr = TWC::ContentAnalysis::ClassifyCurrentMap();
            return dscr & DescriptorMask::Continents();
        };
    case Options::CurrentExpansionMapsOnly:
        return []() {
            auto dscr = TWC::ContentAnalysis::ClassifyCurrentMap();
            // if (dscr.count(ContentFeature::EXPANSION_None) ||
            //     (dscr.count(ContentFeature::EXPANSION_Optional) && UseAccessColours))
            //     return {Strings::CentralTyria, Core};
            return dscr & (DescriptorMask::Expansions() | ContentFeature::EXPANSION_None);
        };
    case Options::AllMapsWithCompletionReward:
        return []() { return ContentFeature::REWARD_ZoneCompletionReward; };
    }
    return []() { return ContentDescriptor(flags::empty_t{}); };
}
void TWC::CompletionManager::Update(Options::CompletionMode wrld, Options::CompletionMode sel,
                                    Options::ExpansionAssignmentMode exp)
{
    CoreMask = ContentDescriptor(ContentFeature::EXPANSION_None);
    if (exp == Options::ExpansionAssignmentMode::MapAccessibility)
        CoreMask |= ContentFeature::EXPANSION_Optional;
    WorldCompletionMask = MakeMask(wrld);
    CharacterSelectionMask = sel != wrld ? MakeMask(sel) : WorldCompletionMask;
}
