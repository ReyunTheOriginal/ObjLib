#pragma once

#include "Component.hpp"
#include <vector>
#include "Math.hpp"
#include "GlobalTypes.hpp"

namespace obj{
    struct collider : component{
        vector2 Offsit = {0,0};
        color DebugColor = {0,255,0,255};

        std::vector<vector2> Vertices = {
            {-0.5, -0.5},
            {-0.5,  0.5},
            { 0.5,  0.5},
            { 0.5, -0.5} 
        };

        std::vector<vector2> GetWorldVertices();
        void DebugDraw(window* Window) override;
    };
}