#pragma once

#include "Completion.hpp"

namespace TWC
{
struct MapCompletion
{
    Completion Landmarks;
    Completion HeroChallenges;
    Completion Tasks;
    Completion Waypoints;
    Completion Vistas;
    Completion Total;

    MapCompletion &operator+=(const MapCompletion &other)
    {
        this->Landmarks += other.Landmarks;
        this->HeroChallenges += other.HeroChallenges;
        this->Tasks += other.Tasks;
        this->Waypoints += other.Waypoints;
        this->Vistas += other.Vistas;
        this->Total += other.Total;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const MapCompletion &mc)
    {
        os << "  Landmarks:       " << mc.Landmarks << "\n"
           << "  Hero Challenges: " << mc.HeroChallenges << "\n"
           << "  Tasks:           " << mc.Tasks << "\n"
           << "  Waypoints:       " << mc.Waypoints << "\n"
           << "  Vistas:          " << mc.Vistas << "\n"
           << "  Total:           " << mc.Total;
        return os;
    }
};
} // namespace TWC
