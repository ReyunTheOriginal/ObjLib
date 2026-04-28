#pragma once

#include "GlobalTypes.hpp"
#include "Sprite.hpp"
#include <unordered_map>

namespace obj{
    struct gameObject;

    namespace Internal{
        struct renderLayer{
            int Order;
            int Layer;
        };

        struct component{
            gameObject* GameObject = nullptr;
            bool Enabled = true;
            bool DidInit = false;

            virtual ~component() = default;
            virtual void Run() {} //any updating that needs to be done
            virtual void Draw(SDL_Renderer* renderer) {} //any rendering
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
                std::unordered_map<SDL_Renderer*, SDL_Texture*> SDLtextures;

                public:
                color Color = {255,255,255};
                sprite* Sprite = nullptr;

                Internal::renderLayer RenderLayer;

                void Run() override;
                void Draw(SDL_Renderer* renderer) override;
                void Destroy() override;

                ~spriteRenderer(){
                    for (auto& SDLtexture : SDLtextures)
                        if (SDLtexture.second)SDL_DestroyTexture(SDLtexture.second);
                }
            };
        #pragma endregion <spriteRenderer>
////////////////////////////////////////////////////
    #pragma endregion <Components>
}