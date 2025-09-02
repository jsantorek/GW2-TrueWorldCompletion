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

    constexpr bool operator==(Colour4 other)
    {
        return Value == other.Value;
    }

    static consteval Colour4 rgb(uint8_t r, uint8_t g, uint8_t b)
    {
        return Colour4(b, g, r, 0xFF);
    }

    static constexpr Colour4 rgba(const float *rgba_f)
    {
        uint8_t rgba[4];
        for (auto i = 0; i < 4; i++)
        {
            float col = rgba_f[i];
            if (col > 1.)
                col = 1.;
            else if (col < 0.)
                col = 0.;
            rgba[i] = static_cast<uint8_t>(0xFF * col);
        }
        return Colour4(rgba[2], rgba[1], rgba[0], rgba[3]);
    }
    void to_rgba(float *rgba) const
    {
        rgba[0] = static_cast<float>(R) / 255.0f;
        rgba[1] = static_cast<float>(G) / 255.0f;
        rgba[2] = static_cast<float>(B) / 255.0f;
        rgba[3] = static_cast<float>(A) / 255.0f;
    }
};
} // namespace TWC
