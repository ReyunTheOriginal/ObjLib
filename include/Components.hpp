#pragma once

#include "GlobalTypes.hpp"
#include <unordered_map>

namespace obj{
    struct gameObject;
    struct sprite;

    namespace Internal{
        struct renderLayer{
            int Order;
            int Layer;
        };

        struct component{
            gameObject* GameObject = nullptr;
            bool Enabled = true;
            Internal::renderLayer* RenderLayer;
            bool DidInit = false;

            virtual ~component() = default;
            virtual void Run() {} //any updating that needs to be done
            virtual void Draw(SDL_Renderer* renderer) {} //any rendering
            virtual void DebugDraw(SDL_Renderer* renderer) {} //any rendering in the window debug mode
            virtual void Init() {} //any init that needs to be ran once
            virtual void Destroy() {delete this;} //what happens when the component is destroyed
        };
    }

    #pragma region <Components>
///////////////////////////////////////////////
        #pragma region <spriteRenderer>
            struct spriteRenderer : Internal::component{
                color Color = {255,255,255};
                sprite* Sprite = nullptr;

                void Run() override;
                void Draw(SDL_Renderer* renderer) override;
            };
        #pragma endregion <spriteRenderer>
////////////////////////////////////////////////////
    #pragma endregion <Components>
}