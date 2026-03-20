#pragma once

#include <cstdint>
#include <ostream>

namespace TWC
{
struct CompletionValue
{
    uint32_t Completed = 0;
    uint32_t Available = 0;

    CompletionValue &operator+=(const CompletionValue &other)
    {
        this->Available += other.Available;
        this->Completed += other.Completed;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const CompletionValue &c)
    {
        os << c.Completed << "/" << c.Available;
        return os;
    }

    operator float() const
    {
        if (Completed >= Available)
            return 1.0f;
        return static_cast<float>(Completed) / static_cast<float>(Available);
    }

    operator uint8_t() const
    {
        if (Completed >= Available)
            return 255;
        return static_cast<uint8_t>(254.0f * static_cast<float>(*this));
    }
};

inline CompletionValue operator+(const CompletionValue &a, const CompletionValue &b)
{
    return CompletionValue{a.Completed + b.Completed, a.Available + b.Available};
}
} // namespace TWC
