#pragma once
#include <cstdint>
namespace TWC
{
struct CompletionByte
{
    uint8_t Completed;
    static constexpr uint8_t Available = 0xFF;
    CompletionByte(uint8_t c) : Completed(c)
    {
    }
    inline operator float() const
    {
        return static_cast<float>(Completed) / static_cast<float>(Available);
    }
};
} // namespace TWC