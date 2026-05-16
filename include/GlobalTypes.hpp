#pragma once

#include "Math.hpp"
#include "SDL3/SDL.h"

namespace obj{

    using rect = SDL_FRect;
    using FlipMode = SDL_FlipMode;

    struct pivot {
        vector2 Point = {0.5f, 0.5f};

        pivot(float X, float Y) : Point{X, Y} {}

        // Just declare them here
        static const pivot TopLeft;
        static const pivot TopRight;
        static const pivot BottomLeft;
        static const pivot BottomRight;
        static const pivot TopCenter;
        static const pivot BottomCenter;
        static const pivot LeftCenter;
        static const pivot RightCenter;
        static const pivot Center;
    };

    // Define them after the struct is complete
    inline const pivot pivot::TopLeft     {0.0f, 0.0f};
    inline const pivot pivot::TopRight    {1.0f, 0.0f};
    inline const pivot pivot::BottomLeft  {0.0f, 1.0f};
    inline const pivot pivot::BottomRight {1.0f, 1.0f};
    inline const pivot pivot::TopCenter   {0.5f, 0.0f};
    inline const pivot pivot::BottomCenter{0.5f, 1.0f};
    inline const pivot pivot::LeftCenter  {0.0f, 0.5f};
    inline const pivot pivot::RightCenter {1.0f, 0.5f};
    inline const pivot pivot::Center      {0.5f, 0.5f};

    namespace Internal{
        struct renderSorter{
            int Order = 0;
            int Layer = 0;
        };
    }
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

    #pragma region <Operators>
            color operator+ (const color other)const{
                return {r + other.r, g + other.g, b + other.b, a + other.a};
            }
            color operator- (const color other)const{
                return {r - other.r, g - other.g, b - other.b, a - other.a};
            }

            color operator* (const color other)const{
                return {r * other.r, g * other.g, b * other.b, a * other.a};
            }
            color operator/ (const color other)const{
                return {r / other.r, g / other.g, b / other.b, a / other.a};
            }

            color operator* (const float other)const{
                return {r * other, g * other, b * other, a * other};
            }
            color operator/ (const float other)const{
                return {r / other, g / other, b / other, a / other};
            }
            color& operator*=(const float other) {
                r *= other; g *= other;b *= other; a *= other;
                return *this;
            }
            color& operator/=(const float other) {
                r /= other; g /= other;b /= other; a /= other;
                return *this;
            }
            color& operator+=(const float other) {
                r += other; g += other; b += other; a += other;
                return *this;
            }
            color& operator-=(const float other) {
                r -= other; g -= other; b -= other; a -= other;
                return *this;
            }

            bool operator==(const color& other) const {
                return r == other.r && g == other.g && b == other.b && a == other.a;
            }

            color operator+= (const color other){
                r += other.r; g+= other.g;b += other.b; a+= other.a;
                return *this;
            }
            color operator-= (const color other){
                r -= other.r; g-= other.g;b -= other.b; a-= other.a;
                return *this;
            }
            color operator*= (const color other){
                r *= other.r; g*= other.g;b *= other.b; a*= other.a;
                return *this;
            }
            color operator/= (const color other){
                r /= other.r; g /= other.g;b /= other.b; a /= other.a;
                return *this;
            }
        #pragma endregion

        static color FromHex(unsigned int hexValue, float alpha = 255) {
            float r = (float)((hexValue >> 16) & 0xFF);
            float g = (float)((hexValue >> 8) & 0xFF);
            float b = (float)(hexValue & 0xFF);
            return color(r, g, b, alpha);
        }

        color(float red, float green, float blue, float alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
        // Example usage: color myColor = color::FromHex(0xFF0000);  // Red
    };
}