#pragma once

#include "Component.hpp"
#include "GlobalTypes.hpp"

struct SDL_Renderer;

namespace obj{
    struct sprite;

    struct spriteRenderer : Internal::component{
        color Color = {255,255,255,255};
        sprite* Sprite = nullptr;

        bool FlipHorizontal = false;
        bool FlipVertical = false;

        void Run() override;
        void Draw(SDL_Renderer* renderer) override;
    };
}