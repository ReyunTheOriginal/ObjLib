#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <type_traits>

namespace obl{

float Pow(float num, float power = 2.0);

struct vector2{
    float x = 0.0,y = 0.0;

    vector2(float X = 0, float Y = 0) : x(X), y(Y){};

    #pragma region <Operators>
        vector2 operator+ (const vector2 other)const{
            return {x + other.x, y + other.y};
        }
        vector2 operator- (const vector2 other)const{
            return {x - other.x, y - other.y};
        }

        vector2 operator* (const vector2 other)const{
            return {x * other.x, y * other.y};
        }
        vector2 operator/ (const vector2 other)const{
            return {x / other.x, y / other.y};
        }

        vector2 operator* (const float other)const{
            return {x * other, y * other};
        }
        vector2 operator/ (const float other)const{
            return {x / other, y / other};
        }
        vector2& operator*=(const float other) {
            x *= other; y *= other;
            return *this;
        }
        vector2& operator/=(const float other) {
            x /= other; y /= other;
            return *this;
        }


        vector2 operator+= (const vector2 other){
            x += other.x; y+= other.y;
            return *this;
        }
        vector2 operator-= (const vector2 other){
            x -= other.x; y-= other.y;
            return *this;
        }
        vector2 operator*= (const vector2 other){
            x *= other.x; y*= other.y;
            return *this;
        }
        vector2 operator/= (const vector2 other){
            x /= other.x; y /= other.y;
            return *this;
        }
    #pragma endregion

    float mag(){return std::sqrt(Pow(x) + Pow(y));}

    vector2 normalized();
};

namespace Math{
    static double PI = 3.141592653;
//
    float Distance(const vector2& A, const vector2& B);
//
    float DistanceSqrt(const vector2& A, const vector2& B);
//
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    Abs(T value){return std::abs(value);}
//
    inline vector2 Abs(const vector2& value){return {Abs(value.x), Abs(value.y)};}
//
    constexpr inline float Dot(const vector2& A, const vector2& B){return A.x * B.x + A.y * B.y;}
//
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    Clamp(T value, T min, T max){return std::clamp(value, min, max);}
//
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    Clamp01(T value){return std::clamp(value, 0, 1);}
//
    template<typename T>
    constexpr inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    Lerp(T A, T B, float t){t = Clamp(t, 0.0f, 1.0f);return A + (B - A) * t;}
//
    inline vector2 Lerp(const vector2& A, const vector2& B, float t){t = Clamp(t, 0.0f, 1.0f); return A + (B - A) * t;}
//
    inline float SmoothStep(float t){return t * t * (3 - 2 * t);}
    inline float SmootherStep(float t){return t * t * t * (t * (t * 6 - 15) + 10);}
    inline float EaseInQuad(float t){return t * t;}
    inline float EaseOutQuad(float t){return 1 - (1 - t) * (1 - t);}
//
    template<typename T>
    constexpr inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    CubicLerp(T A, T B, float t){t = SmoothStep(t);return A + (B - A) * t;}
//
    inline vector2 CubicLerp(const vector2& A, const vector2& B, float t){t = SmoothStep(t);return A + (B - A) * t;}
//
    constexpr inline float Rad2Deg(float value){ return value * (180.0 /3.141592653);}
    constexpr inline float Deg2Rad(float value){ return value * (3.141592653 / 180.0);}
//
    template<typename T>
    constexpr inline int
    Sign(T value){if (value > 0)return 1; if (value < 0) return -1; return 0;}
//
    template<typename T>
    constexpr inline T
    Min(T A, T B){return std::min(A, B);}
//
    template<typename T>
    constexpr inline T
    Max(T A, T B){return std::max(A, B);}
//
    constexpr inline float Floor(float value){return std::floor(value);}
    constexpr inline float Ceil(float value){return std::ceil(value);}
    constexpr inline float Round(float value){return std::round(value);}
//
    constexpr inline float Fract(float v) {return v - std::floor(v);}
}

}