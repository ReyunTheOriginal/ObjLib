#pragma once

#include "Component.hpp"
#include "GlobalTypes.hpp"

struct SDL_Renderer;

namespace obj{
    struct sprite;

    struct spriteRenderer : component{
        color Color = {255,255,255,255};
        sprite* Sprite = nullptr;

        bool FlipHorizontal = false;
        bool FlipVertical = false;

        void Draw(window* Window) override;
    };
}