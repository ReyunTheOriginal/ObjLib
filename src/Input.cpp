#include "Input.hpp"
#include "HelperFunctions.hpp"

namespace obl{
    namespace Internal{
        #pragma region <Input>
        void input::Update(){
            MouseMotionV = {0,0};
            ScrollY = ScrollX = 0.0;

            KeyBPressed.clear();
            KeyBReleased.clear();

            MouseBReleased.clear();
            MouseBPressed.clear();

            while (SDL_PollEvent(&Event)){
                if (Event.type == SDL_QUIT) Quit();

                if (Event.type == SDL_KEYDOWN){
                    KeyBPressed[Event.key.keysym.sym] = true;
                }
                if (Event.type == SDL_KEYUP && Event.key.repeat == 0){
                    KeyBReleased[Event.key.keysym.sym] = true;
                }

                if (Event.type == SDL_MOUSEBUTTONDOWN){
                    MouseBPressed[Event.button.button] = true;
                }
                if (Event.type == SDL_MOUSEBUTTONUP){
                    MouseBReleased[Event.button.button] = true;
                }

                if (Event.type == SDL_MOUSEMOTION){
                    MouseMotionV = {(float)Event.motion.xrel, (float)Event.motion.yrel};
                }

                if (Event.type == SDL_MOUSEWHEEL){
                    ScrollX = Event.wheel.x;
                    ScrollY = Event.wheel.y;
                }

            }

            const Uint8* state = SDL_GetKeyboardState(NULL);
            DirectionalInput = {0.0,0.0};

            if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])DirectionalInput.y = -1.0;
            if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])DirectionalInput.y = 1.0;

            if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])DirectionalInput.x = 1.0;
            if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])DirectionalInput.x = -1.0;

            int x,y;
            SDL_GetMouseState(&x, &y);
            MousePos = {(float)x,(float)y};

            SDL_GetGlobalMouseState(&x, &y);
            MonitorMousePos = {(float)x,(float)y};

            if (MouseMovement == false && WindowFrozenIn)SDL_WarpMouseInWindow(WindowFrozenIn->SDLWindow, FrozenMousePos.x, FrozenMousePos.y);
        }
        #pragma region <PressChecks>
            bool input::KeyPressed(SDL_Keycode Key){
                if (KeyBPressed.contains(Key)){
                    return KeyBPressed.at(Key);
                }else{
                    return false;
                }
            }
            bool input::KeyReleased(SDL_Keycode Key){
                if (KeyBReleased.contains(Key)){
                    return KeyBReleased.at(Key);
                }else{
                    return false;
                }
            }
            bool input::KeyHeld(SDL_Keycode Key){
                const Uint8* state = SDL_GetKeyboardState(NULL);
                SDL_Scancode scan = SDL_GetScancodeFromKey(Key);
                return state[scan];
            }

            bool input::MousePressed(int Button){
                if (MouseBPressed.contains(Button)){
                    return MouseBPressed.at(Button);
                }else{
                    return false;
                }
            }
            bool input::MouseReleased(int Button){
                if (MouseBReleased.contains(Button)){
                    return MouseBReleased.at(Button);
                }else{
                    return false;
                }
            }
            bool input::MouseHeld(Uint8 Button){
                int x, y;
                Uint8 state = SDL_GetMouseState(&x, &y);
                return state & SDL_BUTTON(Button);
            }
        #pragma endregion <PressChecks>
    
        #pragma region <Getters>
            window* input::GetFocusedWindow(){
                SDL_Window* SDLWin = SDL_GetMouseFocus();
                if (SDLWin == NULL){
                    if (Windows.size() > 0) return Windows[0].get();
                }

                for (auto& win : Windows){
                    if (SDLWin == win->SDLWindow){
                        return win.get();
                    }
                }

                return nullptr;
            }
        #pragma endregion <Getters>

        #pragma endregion <Input>
    }//Internal
    Internal::input Input;

    namespace Internal{
        #pragma region <Cursor>
            void Cursor::CreateTexture(){
                if (Texture) SDL_DestroyTexture(Texture);
                if (Window->SDLRenderer)Texture = IMG_LoadTexture(Window->SDLRenderer, Sprites["Cursor"].c_str());
            }

            void Cursor::Update(){
                if (Locked) Input.MousePos = Position;
            }

            void Cursor::Draw(){
                if (Shown){
                    if (Texture){
                        int X, Y;
                        SDL_QueryTexture(Texture, nullptr, nullptr, &X, &Y);

                        SDL_FRect rect;
                        rect.w = (X * 0.2);
                        rect.h = (Y * 0.2);
                        rect.x = ((Position.x + Offsit.x));
                        rect.y = ((Position.y + Offsit.y));

                        SDL_FPoint center = {rect.w / 2, rect.h / 2};

                        SDL_RenderCopyExF(Window->SDLRenderer, Texture, nullptr, &rect,0.0, &center, SDL_FLIP_NONE);
                    }else{
                        if (!Sprites.contains("Cursor")){
                            Print("Sprite Not Found for Cursor");
                            return;
                        }
                        CreateTexture();

                        if (!Texture)Print("Texture Not Found for 'Cursor'");
                    }
                }
            }

            void Cursor::ToggleCursorLock(bool toggle){
                if (toggle == false)SDL_WarpMouseInWindow(Window->SDLWindow, Position.x, Position.y);
                Position = Input.MousePos;
                SDL_SetRelativeMouseMode(toggle? SDL_TRUE : SDL_FALSE);
                Locked = Shown = toggle;
            }
            
        #pragma endregion
    }
}