#pragma once

#include <SDL3/SDL.h>

namespace obj{
    struct color{
        float r,g,b,a = 255;

    #pragma region <Color Presets>
        static const color White;
        static const color Black;
        static const color Red;
        static const color DarkRed;
        static const color LightRed;
        static const color Green;
        static const color DarkGreen;
        static const color LightGreen;
        static const color Blue;
        static const color DarkBlue;
        static const color LightBlue;
        static const color Cyan;
        static const color DarkCyan;
        static const color LightCyan;
        static const color Magenta;
        static const color DarkMagenta;
        static const color LightMagenta;
        static const color Yellow;
        static const color DarkYellow;
        static const color LightYellow;
        static const color Gray;
        static const color DarkGray;
        static const color LightGray;
        static const color Orange;
        static const color DarkOrange;
        static const color LightOrange;
        static const color Purple;
        static const color DarkPurple;
        static const color LightPurple;
        static const color Transparent;
    #pragma endregion <Color Presets>

        SDL_Color ToSDL() const {
            return {
                (Uint8)(r),
                (Uint8)(g),
                (Uint8)(b),
                (Uint8)(a)
            };
        }

        static color FromHex(unsigned int hexValue, float alpha = 255) {
            float r = (float)((hexValue >> 16) & 0xFF);
            float g = (float)((hexValue >> 8) & 0xFF);
            float b = (float)(hexValue & 0xFF);
            return color((int)r, (int)g, (int)b, (int)alpha);
        }

        color(int red, int green, int blue, int alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
        // Example usage: color myColor = color::FromHex(0xFF0000);  // Red
    };
}