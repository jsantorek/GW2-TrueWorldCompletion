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

    operator float()
    {
        if (Completed >= Available)
            return 1.0f;
        return static_cast<float>(Completed) / static_cast<float>(Available);
    }

    operator uint8_t()
    {
        if (Completed >= Available)
            return 255;
        return static_cast<uint8_t>(254.0f * static_cast<float>(*this));
    }
};

inline Completion operator+(const Completion &a, const Completion &b)
{
    return Completion{a.Completed + b.Completed, a.Available + b.Available};
}
} // namespace TWC
