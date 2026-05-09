#pragma once
#include <SDL3/SDL.h>

namespace obj{
    struct InputCode{
        // Letters
        static constexpr SDL_Keycode A = SDLK_A;
        static constexpr SDL_Keycode B = SDLK_B;
        static constexpr SDL_Keycode C = SDLK_C;
        static constexpr SDL_Keycode D = SDLK_D;
        static constexpr SDL_Keycode E = SDLK_E;
        static constexpr SDL_Keycode F = SDLK_F;
        static constexpr SDL_Keycode G = SDLK_G;
        static constexpr SDL_Keycode H = SDLK_H;
        static constexpr SDL_Keycode I = SDLK_I;
        static constexpr SDL_Keycode J = SDLK_J;
        static constexpr SDL_Keycode K = SDLK_K;
        static constexpr SDL_Keycode L = SDLK_L;
        static constexpr SDL_Keycode M = SDLK_M;
        static constexpr SDL_Keycode N = SDLK_N;
        static constexpr SDL_Keycode O = SDLK_O;
        static constexpr SDL_Keycode P = SDLK_P;
        static constexpr SDL_Keycode Q = SDLK_Q;
        static constexpr SDL_Keycode R = SDLK_R;
        static constexpr SDL_Keycode S = SDLK_S;
        static constexpr SDL_Keycode T = SDLK_T;
        static constexpr SDL_Keycode U = SDLK_U;
        static constexpr SDL_Keycode V = SDLK_V;
        static constexpr SDL_Keycode W = SDLK_W;
        static constexpr SDL_Keycode X = SDLK_X;
        static constexpr SDL_Keycode Y = SDLK_Y;
        static constexpr SDL_Keycode Z = SDLK_Z;

        static constexpr int LeftMouseButton   = SDL_BUTTON_LEFT;
        static constexpr int MiddleMouseButton = SDL_BUTTON_MIDDLE;
        static constexpr int RightMouseButton  = SDL_BUTTON_RIGHT;

        // Numbers
        static constexpr SDL_Keycode Num0 = SDLK_0;
        static constexpr SDL_Keycode Num1 = SDLK_1;
        static constexpr SDL_Keycode Num2 = SDLK_2;
        static constexpr SDL_Keycode Num3 = SDLK_3;
        static constexpr SDL_Keycode Num4 = SDLK_4;
        static constexpr SDL_Keycode Num5 = SDLK_5;
        static constexpr SDL_Keycode Num6 = SDLK_6;
        static constexpr SDL_Keycode Num7 = SDLK_7;
        static constexpr SDL_Keycode Num8 = SDLK_8;
        static constexpr SDL_Keycode Num9 = SDLK_9;

        // Function keys
        static constexpr SDL_Keycode F1 = SDLK_F1;
        static constexpr SDL_Keycode F2 = SDLK_F2;
        static constexpr SDL_Keycode F3 = SDLK_F3;
        static constexpr SDL_Keycode F4 = SDLK_F4;
        static constexpr SDL_Keycode F5 = SDLK_F5;
        static constexpr SDL_Keycode F6 = SDLK_F6;
        static constexpr SDL_Keycode F7 = SDLK_F7;
        static constexpr SDL_Keycode F8 = SDLK_F8;
        static constexpr SDL_Keycode F9 = SDLK_F9;
        static constexpr SDL_Keycode F10 = SDLK_F10;
        static constexpr SDL_Keycode F11 = SDLK_F11;
        static constexpr SDL_Keycode F12 = SDLK_F12;

        // Arrows
        static constexpr SDL_Keycode Up = SDLK_UP;
        static constexpr SDL_Keycode Down = SDLK_DOWN;
        static constexpr SDL_Keycode Left = SDLK_LEFT;
        static constexpr SDL_Keycode Right = SDLK_RIGHT;

        // Modifiers
        static constexpr SDL_Keycode LShift = SDLK_LSHIFT;
        static constexpr SDL_Keycode RShift = SDLK_RSHIFT;
        static constexpr SDL_Keycode LCtrl = SDLK_LCTRL;
        static constexpr SDL_Keycode RCtrl = SDLK_RCTRL;
        static constexpr SDL_Keycode LAlt = SDLK_LALT;
        static constexpr SDL_Keycode RAlt = SDLK_RALT;

        // Control keys
        static constexpr SDL_Keycode Escape = SDLK_ESCAPE;
        static constexpr SDL_Keycode Tab = SDLK_TAB;
        static constexpr SDL_Keycode CapsLock = SDLK_CAPSLOCK;
        static constexpr SDL_Keycode Enter = SDLK_RETURN;
        static constexpr SDL_Keycode Backspace = SDLK_BACKSPACE;
        static constexpr SDL_Keycode Space = SDLK_SPACE;

        // Editing
        static constexpr SDL_Keycode Insert = SDLK_INSERT;
        static constexpr SDL_Keycode Delete = SDLK_DELETE;
        static constexpr SDL_Keycode Home = SDLK_HOME;
        static constexpr SDL_Keycode End = SDLK_END;
        static constexpr SDL_Keycode PageUp = SDLK_PAGEUP;
        static constexpr SDL_Keycode PageDown = SDLK_PAGEDOWN;

        // Symbols
        static constexpr SDL_Keycode Minus = SDLK_MINUS;
        static constexpr SDL_Keycode Equals = SDLK_EQUALS;
        static constexpr SDL_Keycode LeftBracket = SDLK_LEFTBRACKET;
        static constexpr SDL_Keycode RightBracket = SDLK_RIGHTBRACKET;
        static constexpr SDL_Keycode Backslash = SDLK_BACKSLASH;
        static constexpr SDL_Keycode Semicolon = SDLK_SEMICOLON;
        static constexpr SDL_Keycode Apostrophe = SDLK_APOSTROPHE;
        static constexpr SDL_Keycode Comma = SDLK_COMMA;
        static constexpr SDL_Keycode Period = SDLK_PERIOD;
        static constexpr SDL_Keycode Slash = SDLK_SLASH;
        static constexpr SDL_Keycode Grave = SDLK_GRAVE;

        // Numpad
        static constexpr SDL_Keycode KP0 = SDLK_KP_0;
        static constexpr SDL_Keycode KP1 = SDLK_KP_1;
        static constexpr SDL_Keycode KP2 = SDLK_KP_2;
        static constexpr SDL_Keycode KP3 = SDLK_KP_3;
        static constexpr SDL_Keycode KP4 = SDLK_KP_4;
        static constexpr SDL_Keycode KP5 = SDLK_KP_5;
        static constexpr SDL_Keycode KP6 = SDLK_KP_6;
        static constexpr SDL_Keycode KP7 = SDLK_KP_7;
        static constexpr SDL_Keycode KP8 = SDLK_KP_8;
        static constexpr SDL_Keycode KP9 = SDLK_KP_9;

        static constexpr SDL_Keycode KPPlus = SDLK_KP_PLUS;
        static constexpr SDL_Keycode KPMinus = SDLK_KP_MINUS;
        static constexpr SDL_Keycode KPMultiply = SDLK_KP_MULTIPLY;
        static constexpr SDL_Keycode KPDivide = SDLK_KP_DIVIDE;
        static constexpr SDL_Keycode KPEnter = SDLK_KP_ENTER;
        static constexpr SDL_Keycode KPPeriod = SDLK_KP_PERIOD;
    };

}