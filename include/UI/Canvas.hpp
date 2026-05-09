#pragma once

#include <vector>
#include <algorithm>
#include "Camera/Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/UITransform.hpp"

namespace obj{
    struct gameObject;

    namespace UI{

        struct canvas{
            private:
            std::vector<screenObject*> ScreenObjects;
            std::vector<screenObject*> ParentlessScreenObjects;

            camera* Camera = nullptr;

            friend canvas* camera::SetCanvas(canvas* Canvas);
            friend screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent);
            friend screenObject::~screenObject();
            friend void Internal::transformUI::SetParent(transformUI* ParentToSet);

            public:
            bool Enabled = true;

            camera* GetCamera(){return Camera;}

            std::vector<screenObject*> GetScreenObjects(){return ScreenObjects;}
            std::vector<screenObject*> GetParentlessScreenObjects(){return ParentlessScreenObjects;}

            canvas(){}
            ~canvas();
        };

        canvas* CreateCanvas();

    }//UI
}//obj