#pragma once

#include <SDL3/SDL.h>

namespace obj{
    struct color{
        float r,g,b,a = 255;

        static const color white;
        static const color black;
        static const color red;
        static const color green;
        static const color blue;
        static const color transparent;

        SDL_Color ToSDL() const {
            return {
                (Uint8)(r),
                (Uint8)(g),
                (Uint8)(b),
                (Uint8)(a)
            };
        }

        color(int red, int green, int blue, int alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    };
}