#pragma once

#include <vector>
#include <algorithm>

namespace obj{
    struct camera;

    namespace UI{
        struct screenObject;

        namespace Internal{
            struct screenComponent;
        }

        struct canvas{
            bool Enabled = true;
            std::vector<screenObject*> UI;

            camera* Camera = nullptr;

            ~canvas();
        };

        canvas* CreateCanvas(camera* Camera);

    }//UI
}//obj