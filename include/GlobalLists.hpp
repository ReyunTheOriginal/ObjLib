#pragma once

#include <unordered_map>

namespace obj{
    struct gameobject;
    struct scene;
    struct window;

    namespace Internal{
        extern int GID;
        extern int SID;
        extern int WID;

        extern std::unordered_map<int, gameobject*> GlobalGameObjects;
        extern std::unordered_map<int, scene*> GlobalScenes;
        extern std::unordered_map<int, window*> GlobalWindows;

    }
} // namespace obj
