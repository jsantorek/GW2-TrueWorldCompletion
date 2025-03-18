#pragma once

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <sstream>

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

    template <typename T> operator std::basic_string<T, std::char_traits<T>, std::allocator<T>>()
    {
        std::basic_stringstream<T, std::char_traits<T>, std::allocator<T>> os;
        os << std::fixed << std::setprecision(1)
           << 100.0f * static_cast<float>(Completed) / static_cast<float>(Available) << "%";
        return os.str();
    }
};

inline Completion operator+(const Completion &a, const Completion &b)
{
    return Completion{a.Completed + b.Completed, a.Available + b.Available};
}
} // namespace TWC
