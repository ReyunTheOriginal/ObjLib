#pragma once

#include <vector>
#include <cstdint>

namespace obj{
    struct gameObject;
    struct scene;
    struct window;
    struct sprite;
    struct font;

    namespace UI{
        struct screenObject;
    }

    namespace Internal{
        extern uint64_t Obj_ID; //incremental ID for Engine Objects

        extern std::vector<gameObject*> GlobalGameObjects;
        extern std::vector<UI::screenObject*> GlobalScreenObjects;
        extern std::vector<scene*> GlobalScenes;
        extern std::vector<window*> GlobalWindows;
        extern std::vector<sprite*> GlobalSprites;
        extern std::vector<font*> GlobalFonts;

    }
} // namespace obj
