#pragma once

#include "GlobalTypes.hpp"
#include "UI/ScreenComponent.hpp"

namespace obj{
    struct sprite;

    namespace UI{
        namespace Internal{
            struct screenComponent;
        }

        struct image : screenComponent{
            color Color = {255,255,255,255};
            sprite* Sprite = nullptr;

            bool FlipHorizontal = false;
            bool FlipVertical = false;

            void Draw(window* Window) override;
        };
    }
}