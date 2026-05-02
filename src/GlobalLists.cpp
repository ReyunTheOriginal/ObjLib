#include "GlobalLists.hpp"

#include <limits>

namespace obj{
    namespace Internal{
        int Obj_ID = std::numeric_limits<int>::min(); //incremental ID for Engine Objects

        std::vector<gameObject*> GlobalGameObjects;
        std::vector<UI::screenObject*> GlobalScreenObjects;
        std::vector<scene*> GlobalScenes;
        std::vector<window*> GlobalWindows;
        std::vector<sprite*> GlobalSprites;
    }
} // namespace obj
