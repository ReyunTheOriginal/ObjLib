#pragma once

#include <vector>

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

            std::vector<Internal::screenComponent*> RenderableUI;

            camera* Camera = nullptr;
        };

        canvas* CreateCanvas(camera* Camera);

    }//UI
}//obj