#pragma once

#include <cstdint>
#include <ostream>

namespace TWC
{
struct Completion
{
    int32_t Completed = 0;
    int32_t Available = 0;

    Completion &operator+=(const Completion &other)
    {
        this->Available += other.Available;
        this->Completed += other.Completed;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Completion &c)
    {
        os << c.Completed << "/" << c.Available;
        return os;
    }
};

inline Completion operator+(const Completion &a, const Completion &b)
{
    return Completion{.Completed = a.Completed + b.Completed, .Available = a.Available + b.Available};
}
} // namespace TWC
