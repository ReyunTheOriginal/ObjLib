#pragma once

#include <vector>
#include <utility>

namespace obj{
    struct gameObject;
    struct scene;
    struct window;
    struct sprite;

    namespace Internal{
        extern int Gam_ID; //incremental ID for GameObjects
        extern int Sce_ID; //incremental ID for Scenes
        extern int Win_ID; //incremental ID for Windows
        extern int Spr_ID; //incremental ID for Sprites

        extern std::vector<gameObject*> GlobalGameObjects;
        extern std::vector<scene*> GlobalScenes;
        extern std::vector<window*> GlobalWindows;
        extern std::vector<sprite*> GlobalSprites;

    }
} // namespace obj
