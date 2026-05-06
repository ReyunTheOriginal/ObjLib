#pragma once

#include <vector>
#include <algorithm>
#include "UI/ScreenObject.hpp"

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

            Internal::transformUI* ObjectParent = nullptr;

            camera* Camera = nullptr;

            canvas(){ObjectParent = new Internal::transformUI(nullptr);}
            ~canvas();
        };

        canvas* CreateCanvas(camera* Camera);

    }//UI
}//obj