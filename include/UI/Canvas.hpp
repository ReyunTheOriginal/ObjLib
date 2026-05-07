#pragma once

#include <vector>
#include <algorithm>

namespace obj{
    struct camera;
    struct gameObject;

    namespace UI{
        struct screenObject;

        struct canvas{
            bool Enabled = true;
            std::vector<screenObject*> ScreenObjects;
            std::vector<screenObject*> ParentlessScreenObjects;

            camera* Camera = nullptr;

            canvas(){}
            ~canvas();
        };

        canvas* CreateCanvas(camera* Camera);

    }//UI
}//obj