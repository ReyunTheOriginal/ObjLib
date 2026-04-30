#include "Input.hpp"

#include "GlobalLists.hpp"
#include "Math.hpp"
#include "InputKeys.hpp"
#include "EssentialProcesses.hpp"
#include "Window.hpp"
#include "Camera.hpp"

namespace obj{
    namespace Input{
        
        namespace Internal{
            std::unordered_set<SDL_Keycode> KeysPressed;
            std::unordered_set<SDL_Keycode> KeysReleased;
            std::unordered_set<SDL_Keycode> KeysHeld;

            std::unordered_set<Uint8> MouseButtonsPressed;
            std::unordered_set<Uint8> MouseButtonsReleased;
            std::unordered_set<Uint8> MouseButtonsHeld;
        }

    vector2 ScreenMousePosition = {0,0};
    vector2 WorldMousePosition = {0,0};
    vector2 GlobalMousePosition = {0,0};
    vector2 MouseMotion = {0,0};
    vector2 DirectionalInput = {0,0};
    window* FocusedWindow = nullptr;

    void Update(){
        Internal::KeysPressed.clear();
        Internal::KeysReleased.clear();

        Internal::MouseButtonsPressed.clear();
        Internal::MouseButtonsReleased.clear();

        MouseMotion = {0,0};
        DirectionalInput = {0,0};

        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    Quit();
                    break;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    Quit();
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.repeat == 0) Internal::KeysPressed.insert(event.key.key);
                    Internal::KeysHeld.insert(event.key.key);
                    break;
                case SDL_EVENT_KEY_UP:
                    Internal::KeysReleased.insert(event.key.key);
                    Internal::KeysHeld.erase(event.key.key);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    MouseMotion = {event.motion.xrel, event.motion.yrel};
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    Internal::MouseButtonsPressed.insert(event.button.button);
                    Internal::MouseButtonsHeld.insert(event.button.button);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    Internal::MouseButtonsReleased.insert(event.button.button);
                    Internal::MouseButtonsHeld.erase(event.button.button);
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    Internal::KeysHeld.clear();
                    Internal::MouseButtonsHeld.clear();
                    break;
                default:
                    break;
            }
        }

        SDL_GetMouseState(&ScreenMousePosition.x, &ScreenMousePosition.y);
        SDL_GetGlobalMouseState(&GlobalMousePosition.x, &GlobalMousePosition.y);

        // Get The Focused window
        SDL_Window* FoWindow = SDL_GetMouseFocus();
        if (FoWindow){
            //loop through all windows and get the one with the correct sdlWindow
            for (auto& win : ::obj::Internal::GlobalWindows){
                if (win->SDLwindow == FoWindow){
                    FocusedWindow = win;
                    break;
                }
            }
        }

        //Adjust the mouse position for resolution difference
        if (FocusedWindow){
            scene* Scene = FocusedWindow->GetScene();
            if (Scene){
                camera* camera = Scene->ActiveCamera;
                camera->ActiveWindow = FocusedWindow;

                // Convert physical mouse coords to logical (letterboxed) render coords
                float logicalX, logicalY;
                SDL_RenderCoordinatesFromWindow(
                    FocusedWindow->SDLrenderer,
                    ScreenMousePosition.x, ScreenMousePosition.y,
                    &logicalX, &logicalY
                );

                WorldMousePosition = camera->ScreenToWorldPosition({logicalX, logicalY});
            }
        }else{
            //default for World Mouse Position
            WorldMousePosition = {-1, -1};
        }

        DirectionalInput.y =
        (KeyHeld(KeyCode::W) || KeyHeld(KeyCode::Up)) -
        (KeyHeld(KeyCode::S) || KeyHeld(KeyCode::Down));

    DirectionalInput.x =
        (KeyHeld(KeyCode::D) || KeyHeld(KeyCode::Right)) -
        (KeyHeld(KeyCode::A) || KeyHeld(KeyCode::Left));
        
    }

    #pragma region <Input Functions>
        bool KeyPressed(SDL_Keycode Key){
            return Internal::KeysPressed.find(Key) != Internal::KeysPressed.end();
        }
        bool KeyReleased(SDL_Keycode Key){
            return Internal::KeysReleased.find(Key) != Internal::KeysReleased.end();
        }
        bool KeyHeld(SDL_Keycode Key){
            return Internal::KeysHeld.find(Key) != Internal::KeysHeld.end();
        }
//////////////////////////////////////////////////////////////////
        bool MouseButtonPressed(int Button){
            return Internal::MouseButtonsPressed.find((Uint8)Button) != Internal::MouseButtonsPressed.end();
        }
        bool MouseButtonReleased(int Button){
            return Internal::MouseButtonsReleased.find((Uint8)Button) != Internal::MouseButtonsReleased.end();
        }
        bool MouseButtonHeld(int Button){
            return Internal::MouseButtonsHeld.find((Uint8)Button) != Internal::MouseButtonsHeld.end();
        }
    #pragma endregion <Input Functions>

    }
}