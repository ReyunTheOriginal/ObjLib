#include "GlobalLists.hpp"

namespace obj{
    namespace Internal{
        int Obj_ID = 0; //incremental ID for Engine Objects

        std::vector<gameObject*> GlobalGameObjects;
        std::vector<UI::screenObject*> GlobalScreenObjects;
        std::vector<scene*> GlobalScenes;
        std::vector<window*> GlobalWindows;
        std::vector<sprite*> GlobalSprites;
    }
} // namespace obj
