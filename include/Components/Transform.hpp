#pragma once

#include "Component.hpp"

namespace obj{
    namespace Internal{
        struct transform : component{
            vector2 Position = {0,0};
            vector2 Scale = {1,1};
            float Rotation = 0;
        };
    }
}