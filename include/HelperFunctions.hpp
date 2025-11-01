#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <random>
#include <type_traits>
#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Math.hpp"
#include "GlobalVar.hpp"
#include "GlobalTypes.hpp"

extern std::mt19937 gen;

namespace obl{
struct gameObject;
struct color;

//starts a window and returns a type that has the SDL window and renderer
window* CreateWindow(std::string Title, vector2 Resulotion, Uint32 SDL_Window_Flags, Uint32 SDL_Renderer_Flags);
gameObject* CreateGameObject(window& Window);

//captialized string helpers
std::string Lower(std::string stri);
std::string Upper(std::string stri);

//Platform Dynamic path to the executale to the app
std::string exePath();

//general print() for any type i can think of
void Print(const auto& P){
    using type = std::decay_t<decltype(P)>;

    if constexpr (std::is_same_v<type, vector2> || std::is_same_v<type, SDL_FPoint> || std::is_same_v<type, SDL_Point>){
        std::cout << "(" << P.x << ", " << P.y << ")" << std::endl;
    }else if constexpr(std::is_same_v<type, gameObject>){
        std::cout << P.Name << std::endl;
    }else if constexpr (std::is_same_v<type, bool>){
        std::cout << (P? "true" : "false") << std::endl;
    }else if constexpr (std::is_arithmetic_v<type>){
        std::cout << P << std::endl;
    }else if constexpr (std::is_same_v<type, std::string>){
        std::cout << P << std::endl;
    }else if constexpr (std::is_convertible_v<type, std::string_view>){
        std::cout << P << std::endl;;
    }else if constexpr (std::is_same_v<type, color>){
        std::cout << "(" << P.red << ", " << P.green << ", " << P.blue << ", " << P.alpha << ")" << std::endl;
    }else{
        std::cout << "[unprintable type: " << typeid(P).name() << "]" << std::endl;
    }
}

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

