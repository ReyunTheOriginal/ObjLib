#pragma once

#include <SDL3/SDL.h>
#include "Math.hpp"
#include <unordered_set>
#include "InputKeys.hpp"
#include "EssentialProcesses.hpp"
#include "Window.hpp"

namespace obj{
    namespace Input{
        
        namespace Internal{
            extern std::unordered_set<SDL_Keycode> KeysPressed;
            extern std::unordered_set<SDL_Keycode> KeysReleased;
            extern std::unordered_set<SDL_Keycode> KeysHeld;

            extern std::unordered_set<Uint8> MouseButtonsPressed;
            extern std::unordered_set<Uint8> MouseButtonsReleased;
            extern std::unordered_set<Uint8> MouseButtonsHeld;
        }

        extern vector2 ScreenMousePosition;
        extern vector2 WorldMousePosition;
        extern vector2 GlobalMousePosition;
        extern vector2 MouseMotion;
        extern vector2 DirectionalInput;
        extern window* FocusedWindow;

        void Update();

        bool KeyPressed(SDL_Keycode Key);
        bool KeyReleased(SDL_Keycode Key);
        bool KeyHeld(SDL_Keycode Key);

        bool MouseButtonPressed(int Button);
        bool MouseButtonReleased(int Button);
        bool MouseButtonHeld(int Button);

    }
}