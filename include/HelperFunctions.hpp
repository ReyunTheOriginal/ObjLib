#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <random>
#include <type_traits>
#include <vector>

#include "GlobalTypes.hpp"
#include "Math.hpp"

extern std::mt19937 gen;

struct SDL_FPoint;
struct SDL_Point;

namespace obj{
    //captialized string helpers
    std::string Lower(std::string stri);
    std::string Upper(std::string stri);

    //Platform Dynamic path to the executale to the app
    std::string exePath();

    //random Generation
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    RandomRange(T min, T max){
        if constexpr (std::is_integral<T>::value){
            std::uniform_int_distribution<int> RandomI(min,max);
            int output = RandomI(gen);
            return output;
        }else{
            std::uniform_real_distribution<T> RandomI(min,max);
            T output = RandomI(gen);
            return output;
        }
    }

    void Destroy(auto& Obj){
        delete Obj;
    }

    //general print() for any type i can think of
    void Print(const auto& P){
        using type = std::decay_t<decltype(P)>;

        if constexpr (std::is_same_v<type, vector2> || std::is_same_v<type, SDL_FPoint> || std::is_same_v<type, SDL_Point>){
            std::cout << "(" << P.x << ", " << P.y << ")" << "\n";
        }else if constexpr (std::is_same_v<type, bool>){
            std::cout << (P? "true" : "false") << "\n";
        }else if constexpr (std::is_arithmetic_v<type>){
            std::cout << P << "\n";
        }else if constexpr (std::is_same_v<type, std::string>){
            std::cout << P << "\n";
        }else if constexpr (std::is_convertible_v<type, std::string_view>){
            std::cout << P << "\n";;
        }else if constexpr (std::is_same_v<type, color>){
            std::cout << "(" << P.r << ", " << P.g << ", " << P.b << ", " << P.a << ")" << "\n";
        }else{
            std::cout << "[unprintable type: " << typeid(P).name() << "]" << "\n";
        }
    }
}