#include "GlobalLists.hpp"

#include <limits>

namespace obj{
    namespace Internal{
        uint64_t Obj_ID = 0; //incremental ID for Engine Objects

        std::vector<gameObject*> GlobalGameObjects;
        std::vector<UI::screenObject*> GlobalScreenObjects;
        std::vector<scene*> GlobalScenes;
        std::vector<window*> GlobalWindows;
        std::vector<sprite*> GlobalSprites;
        std::vector<font*> GlobalFonts;
    }
} // namespace obj
