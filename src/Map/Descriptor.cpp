#include "Map/Descriptor.hpp"
#include "Anomaly/Replace.hpp"
#include "Constant/ContinentIdentifier.hpp"
#include "Constant/RegionIdentifier.hpp"
#include "Game/Access/ContentAccess.h"
#include "Game/Map/EMapFlags.h"
#include "Game/Map/RegionDef.h"
#include "Logging.hpp"
#include "Map/Features.hpp"
#include "Model/Expansion.hpp"
#include "Model/Festival.hpp"

using namespace GW2RE;

TWC::MapDescriptor TWC::MapDescriptor::From(const GW2RE::MapDef_t *def)
{
    TWC::MapDescriptor descriptor;
    if (def->Access)
    {
        switch (def->Access->Expansion)
        {
        case EExpansionAccess::PathOfFire:
            descriptor.Expansion = TWC::Expansion::PathOfFire;
            break;
        case EExpansionAccess::EndOfDragons:
            descriptor.Expansion = TWC::Expansion::EndOfDragons;
            break;
        case EExpansionAccess::SecretsOfTheObscure:
            descriptor.Expansion = TWC::Expansion::SecretsOfTheObscure;
            break;
        case EExpansionAccess::JanthirWilds:
            descriptor.Expansion = TWC::Expansion::JanthirWilds;
            break;
        case EExpansionAccess::VisionsOfEternity:
            descriptor.Expansion = TWC::Expansion::VisionsOfEternity;
            break;
        default:
            LOG(DEBUG, "Map#{} has unidentified expansion (value={}) assigned", def->ID,
                static_cast<uint32_t>(def->Access->Expansion));
        }
    }
    else if (static_cast<uint64_t>(def->Flags & EMapFlags::RequiresHeartOfThorns))
    {
        descriptor.Expansion = TWC::Expansion::HeartOfThorns;
    }
    else
    {
        descriptor.Features.emplace(TWC::MapFeatures::Accessible_FreeAccount);
        auto it = std::find_if(
            TWC::AnomalyReplace::FestivalMaps.begin(), TWC::AnomalyReplace::FestivalMaps.end(),
            [id = static_cast<int>(def->ID)](auto festivalmap) { return std::get<int>(festivalmap) == id; });
        if (it != TWC::AnomalyReplace::FestivalMaps.end())
        {
            descriptor.Features.emplace(TWC::MapFeatures::Accessible_Festival);
            descriptor.Festival = std::get<TWC::Festival>(*it);
        }
        else
        {
            auto jt = std::find_if(
                TWC::AnomalyReplace::ExpansionMaps.begin(), TWC::AnomalyReplace::ExpansionMaps.end(),
                [id = static_cast<int>(def->ID)](auto festivalmap) { return std::get<int>(festivalmap) == id; });
            if (jt != TWC::AnomalyReplace::ExpansionMaps.end())
            {
                descriptor.Expansion = std::get<TWC::Expansion>(*jt);
            }
        }
    }
    if (def->Region)
    {
        if (def->Region->ID == ConstantRegionIdentifier::PvP)
        {
            descriptor.Features.emplace(MapFeatures::Competitive_PvP);
            descriptor.Features.emplace(MapFeatures::Continent_Mists);
        }
        else if (def->Region->ID == ConstantRegionIdentifier::WvW)
        {
            descriptor.Features.emplace(MapFeatures::Competitive_WvW);
            descriptor.Features.emplace(MapFeatures::Continent_Mists);
        }
        else if (def->Region->Continent)
        {
            if (def->Region->Continent->ID == ConstantContinentIdentifier::Mists)
            {
                descriptor.Features.emplace(MapFeatures::Continent_Mists);
            }
            else if (def->Region->Continent->ID == ConstantContinentIdentifier::Tyria)
            {
                descriptor.Features.emplace(MapFeatures::Continent_Tyria);
            }
            else
            {
                LOG(DEBUG, "Map#{} has unidentified continent (id={}) assigned", def->ID,
                    static_cast<uint32_t>(def->Region->Continent->ID));
            }
        }
        else
        {
            LOG(DEBUG, "Region#{} has no continent assigned", def->Region->ID);
        }
    }
    else
    {
        LOG(DEBUG, "Map#{} has no region assigned", def->ID);
    }
    if (static_cast<uint64_t>(def->Flags & EMapFlags::TrackProgress))
    {
        descriptor.Features |= MapFeatures::HasCompletionReward;
    }
    return descriptor;
}
