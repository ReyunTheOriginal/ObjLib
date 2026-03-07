#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <random>
#include <type_traits>
#include <SDL2/SDL.h>
#include <vector>

extern std::mt19937 gen;

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
}