#pragma once

#include<cmath>

namespace obj{
    struct vector2{
        float x = 0,y = 0;
        vector2(float X, float Y){x = X, y = Y;}

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

        float mag(){return std::sqrt((x*x) + (y*y));}
        float Sqrtmag(){return (x*x) + (y*y);}

        vector2 normalized(){
            float Mag = mag();
            if (Mag != 0)return {x/Mag, y/Mag};
            return {0.0,0.0};
        };
    };
}