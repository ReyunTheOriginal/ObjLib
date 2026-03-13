#include "Input.hpp"
#include "GlobalLists.hpp"
#include "Scene.hpp"

namespace obj{
    void Internal::input::Update(){
        KeysPressed.clear();
        KeysReleased.clear();

        MouseButtonsPressed.clear();
        MouseButtonsReleased.clear();

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
                    if (event.key.repeat == 0) KeysPressed.insert(event.key.key);
                    KeysHeld.insert(event.key.key);
                    break;
                case SDL_EVENT_KEY_UP:
                    KeysReleased.insert(event.key.key);
                    KeysHeld.erase(event.key.key);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    MouseMotion = {event.motion.xrel, event.motion.yrel};
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    MouseButtonsPressed.insert(event.button.button);
                    MouseButtonsHeld.insert(event.button.button);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    MouseButtonsReleased.insert(event.button.button);
                    MouseButtonsHeld.erase(event.button.button);
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    KeysHeld.clear();
                    MouseButtonsHeld.clear();
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
            for (auto& win : Internal::GlobalWindows){
                if (win.second->SDLwindow == FoWindow){
                    FocusedWindow = win.second;
                    break;
                }
            }
        }

        //Adjust the mouse position for resolution difference
        if (FocusedWindow){
            camera* camera = FocusedWindow->GetScene()->GetActiveCamera();

            vector2 winres = FocusedWindow->GetResolution();
            vector2 dif = winres-camera->GetResolution();
            ScreenMousePosition -= dif/2;

            //get the world position
            WorldMousePosition = camera->ScreenToWorldPosition(ScreenMousePosition);
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
        bool Internal::input::KeyPressed(SDL_Keycode Key){
            return KeysPressed.find(Key) != KeysPressed.end();
        }
        bool Internal::input::KeyReleased(SDL_Keycode Key){
            return KeysReleased.find(Key) != KeysReleased.end();
        }
        bool Internal::input::KeyHeld(SDL_Keycode Key){
            return KeysHeld.find(Key) != KeysHeld.end();
        }
//////////////////////////////////////////////////////////////////
        bool Internal::input::MouseButtonPressed(int Button){
            return MouseButtonsPressed.find((Uint8)Button) != MouseButtonsPressed.end();
        }
        bool Internal::input::MouseButtonReleased(int Button){
            return MouseButtonsReleased.find((Uint8)Button) != MouseButtonsReleased.end();
        }
        bool Internal::input::MouseButtonHeld(int Button){
            return MouseButtonsHeld.find((Uint8)Button) != MouseButtonsHeld.end();
        }
    #pragma endregion <Input Functions>

    Internal::input Input;
}