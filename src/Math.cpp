#include "Math.hpp"

namespace obl{

vector2 vector2::normalized(){
    float Mag = mag();
    if (Mag != 0)return {x/Mag, y/Mag};

    return {0.0,0.0};
}

float Pow(float num, float power){
    float reserve = 1.0;

    for (int i=0;i<power;i++){
        reserve *= num;
    }
    return reserve;
}

namespace Math{
    float Distance(const vector2& A, const vector2& B){
        return std::sqrt(Pow((B.x - A.x)) + Pow((B.y - A.y)));
    }
    float DistanceSqrt(const vector2& A, const vector2& B){
        return Pow((B.x - A.x)) + Pow((B.y - A.y));
    }

}

}