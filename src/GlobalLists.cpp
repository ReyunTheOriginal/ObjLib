#include "GlobalLists.hpp"

namespace obj{
    namespace Internal{
        int GID = 0;
        int SID = 0;
        int WID = 0;

        std::unordered_map<int, gameobject*> GlobalGameObjects;
        std::unordered_map<int, scene*> GlobalScenes;
        std::unordered_map<int, window*> GlobalWindows;
    }
} // namespace obj
