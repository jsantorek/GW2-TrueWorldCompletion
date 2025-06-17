#pragma once

#include <cstdint>
namespace TWC
{
struct Colour4
{
    union {
        uint32_t Value;
        struct
        {
            uint8_t B, G, R, A;
        };
    };
    constexpr Colour4(uint8_t b = 0xFF, uint8_t g = 0xFF, uint8_t r = 0xFF, uint8_t a = 0xFF) : B(b), G(g), R(r), A(a)
    {
    }

    constexpr explicit Colour4(uint32_t colourValue) : Value(colourValue)
    {
    }

    constexpr operator uint32_t() const noexcept
    {
        return Value;
    }

    static consteval Colour4 rgb(uint8_t r, uint8_t g, uint8_t b)
    {
        return Colour4(b, g, r, 0xFF);
    }
};
} // namespace TWC
