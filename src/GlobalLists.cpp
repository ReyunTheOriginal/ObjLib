#include "GlobalLists.hpp"

namespace obj{
    namespace Internal{
        int GID = 0; //incremental ID for GameObjects
        int SID = 0; //incremental ID for Scenes
        int WID = 0; //incremental ID for Windows

        std::unordered_map<int, gameObject*> GlobalGameObjects;
        std::unordered_map<int, scene*> GlobalScenes;
        std::unordered_map<int, window*> GlobalWindows;
    }
} // namespace obj
