#pragma once

#include <SDL3/SDL.h>

namespace obj{
    struct color{
        float r,g,b,a = 1;

        static const color white;
        static const color black;
        static const color red;
        static const color green;
        static const color blue;
        static const color transparent;

        SDL_Color ToSDL() const {
            return {
                (Uint8)(r * 255),
                (Uint8)(g * 255),
                (Uint8)(b * 255),
                (Uint8)(a * 255)
            };
        }

        color(int red, int green, int blue, int alpha = 255)
        : r(red/255), g(green/255), b(blue/255), a(alpha/255) {}
    };
}