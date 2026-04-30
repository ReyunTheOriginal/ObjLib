#pragma once

#include "GlobalTypes.hpp"
#include "UI/screenComponent.hpp"

namespace obj{
    struct sprite;

    namespace UI{
        namespace Internal{
            struct screenComponent;
        }

        struct image : Internal::screenComponent{
            color Color = {255,255,255,255};
            sprite* Sprite = nullptr;

            bool FlipHorizontal = false;
            bool FlipVertical = false;

            void Run() override;
            void Draw(SDL_Renderer* renderer) override;
        };
    }
}