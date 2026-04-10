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

    [[nodiscard]] float Percent() const
    {
        if (Completed == Available)
            return 100.f;
        float percent = static_cast<float>(*this) * 100.0f;
        constexpr auto RoundingFactor = 10.f;
        return std::floor(percent * RoundingFactor) / RoundingFactor;
    }

    operator float() const
    {
        if (Completed >= Available)
            return 1.0f;
        return static_cast<float>(Completed) / static_cast<float>(Available);
    }
};

inline CompletionValue operator+(const CompletionValue &a, const CompletionValue &b)
{
    return CompletionValue{a.Completed + b.Completed, a.Available + b.Available};
}
} // namespace TWC
