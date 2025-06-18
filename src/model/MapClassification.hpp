#pragma once

#include <cstdint>
#include <ostream>
namespace TWC
{
struct MapClassification
{
    enum MapContinent : uint16_t
    {
        UndeterminedContinent = 0,
        Tyria = 1,
        TheMists = 2
    } Continent = UndeterminedContinent;

    enum MapExpansion : uint16_t
    {
        Core = 0,
        HeartOfThorns = 1,
        PathOfFire = 2,
        EndOfDragons = 3,
        SecretsOfTheObscure = 4,
        JanthirWilds = 5,
        Unidentified
    } Expansion = Unidentified, Chronology = Unidentified;

    operator bool() const
    {
        return Expansion != Unidentified && Chronology != Unidentified && Continent != UndeterminedContinent;
    }

    friend std::ostream &operator<<(std::ostream &os, const MapClassification &c)
    {
        os << "[on " << c.Continent << ", accessible to " << c.Expansion << ", during " << c.Chronology << "]";
        return os;
    }

    enum WorldDivisionMode
    {
        AllMapsCollectively,
        CurrentOrEarlierExpansionMaps,
        CurrentContinentMapsOnly,
        CurrentExpansionMapsOnly
    };

    enum ExpansionAssignmentMode
    {
        MapAccessibility,
        MapReleaseChronology
    };
};
} // namespace TWC
