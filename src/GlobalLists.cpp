#include "GlobalLists.hpp"

namespace obj{
    namespace Internal{
        int Gam_ID = 0; //incremental ID for GameObjects
        int Sce_ID = 0; //incremental ID for Scenes
        int Win_ID = 0; //incremental ID for Windows
        int Spr_ID = 0; //incremental ID for Sprites

        std::vector<gameObject*> GlobalGameObjects;
        std::vector<scene*> GlobalScenes;
        std::vector<window*> GlobalWindows;
        std::vector<sprite*> GlobalSprites;
    }
} // namespace obj
