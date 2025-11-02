#pragma once

#include <unordered_map>
#include <SDL2/SDL.h>

#include "EssientialProccesses.hpp"
#include "Math.hpp"

namespace obl{
    struct window;

    namespace Internal{
        struct Cursor{
            private:
            vector2 Position = {0,0};
            SDL_Texture* Texture = nullptr;
            window* Window = nullptr;
            bool Locked = false;

            void CreateTexture();

            public:
            bool Shown = false;
            vector2 Offsit = {0,0};

            void Update();

            void Draw();

            void Init(window* Win){
                Window = Win;
                CreateTexture();
            }

            void ToggleCursorLock(bool toggle);
        };

        struct input{
            private:
            std::unordered_map<SDL_Keycode, bool> KeyBPressed;
            std::unordered_map<SDL_Keycode, bool> KeyBReleased;

            std::unordered_map<Uint8, bool> MouseBPressed;
            std::unordered_map<Uint8, bool> MouseBReleased;

            vector2 MouseMotionV = {0,0};
            vector2 MousePos = {0,0};
            vector2 MonitorMousePos = {0,0};

            vector2 FrozenMousePos = {0,0};
            window* WindowFrozenIn = nullptr;
            bool MouseMovement = true;

            friend struct Cursor;

            public:
            SDL_Event Event;
            vector2 DirectionalInput = {0,0};
            double ScrollX = 0.0;
            double ScrollY = 0.0;

            void Update();

            vector2 MouseMotion(){return MouseMotionV;}

            bool KeyPressed(SDL_Keycode Key);
            bool KeyReleased(SDL_Keycode Key);
            bool KeyHeld(SDL_Keycode Key);

            bool MousePressed(int Button);
            bool MouseReleased(int Button);
            bool MouseHeld(Uint8 Button);

            vector2 GetMousePos(){return MousePos;}
            vector2 GetMonitorMousePos(){return MonitorMousePos;}

            window* GetFocusedWindow();
            
        };
    }
    
    extern Internal::input Input;
}