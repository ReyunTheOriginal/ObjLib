#pragma once

#include "Component.hpp"
#include "GlobalTypes.hpp"

namespace obj{
    struct sprite;

    struct spriteRenderer : component{
        color Color = {255,255,255,255};
        sprite* Sprite = nullptr;
        pivot Pivot = pivot::Center;

        bool FlipHorizontal = false;
        bool FlipVertical = false;

        void Draw(window* Window) override;
    };
}