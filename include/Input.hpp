#pragma once

#include <SDL3/SDL.h>
#include "Math.hpp"
#include <unordered_set>
#include "InputKeys.hpp"
#include "EssentialProcesses.hpp"

namespace obj{
    namespace Internal{
        struct input{
            private:
            std::unordered_set<SDL_Keycode> KeysPressed;
            std::unordered_set<SDL_Keycode> KeysReleased;
            std::unordered_set<SDL_Keycode> KeysHeld;

            std::unordered_set<Uint8> MouseButtonsPressed;
            std::unordered_set<Uint8> MouseButtonsReleased;
            std::unordered_set<Uint8> MouseButtonsHeld;

            public:
            vector2 MousePosition = {0,0};
            vector2 GlobalMousePosition = {0,0};
            vector2 MouseMotion = {0,0};

            void Update();

            bool KeyPressed(SDL_Keycode Key);
            bool KeyReleased(SDL_Keycode Key);
            bool KeyHeld(SDL_Keycode Key);

            bool MouseButtonPressed(int Button);
            bool MouseButtonReleased(int Button);
            bool MouseButtonHeld(int Button);
        };
    }
    extern Internal::input Input;
}