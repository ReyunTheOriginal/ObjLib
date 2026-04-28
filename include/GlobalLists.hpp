#pragma once

#include <unordered_map>

namespace obj{
    struct gameObject;
    struct scene;
    struct window;

    namespace Internal{
        extern int GID;
        extern int SID;
        extern int WID;

        extern std::unordered_map<int, gameObject*> GlobalGameObjects;
        extern std::unordered_map<int, scene*> GlobalScenes;
        extern std::unordered_map<int, window*> GlobalWindows;

    }
} // namespace obj
