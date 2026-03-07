#pragma once

#include "GlobalTypes.hpp"
#include "Sprite.hpp"

namespace obj{
    struct gameobject;

    namespace Internal{
        struct component{
            gameobject* GameObject = nullptr;
            bool Enabled = true;
            bool DidInit = false;

            virtual ~component() = default;
            virtual void Run() {} //any updating that needs to be done
            virtual void Draw() {} //any rendering
            virtual void DebugDraw() {} //any rendering in the window debug mode
            virtual void Init() {} //any init that needs to be ran once
            virtual void Destroy() {delete this;} //what happens when the component is destroyed
        };
    }

    #pragma region <Components>
///////////////////////////////////////////////
        #pragma region <spriteRenderer>
            struct spriteRenderer : Internal::component{
                private:
                SDL_Texture* SDLtexture = nullptr;

                public:
                color Color = {255,255,255};
                sprite* Sprite = nullptr;

                void Run() override;
                void Draw() override;
                void Destroy() override;

                ~spriteRenderer(){
                    if (SDLtexture)SDL_DestroyTexture(SDLtexture);
                }
            };
        #pragma endregion <spriteRenderer>
////////////////////////////////////////////////////
    #pragma endregion <Components>
}