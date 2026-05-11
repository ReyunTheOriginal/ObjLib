#pragma once

#include <vector>
#include <algorithm>
#include "Camera/Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/UITransform.hpp"

namespace obj{

    namespace UI{

        struct canvas{
            private:
            std::vector<screenObject*> ScreenObjects;
            std::vector<screenObject*> ActiveScreenObjects;
            std::vector<screenObject*> InctiveScreenObjects;
            std::vector<screenObject*> ParentlessScreenObjects;

            camera* Camera = nullptr;

            friend canvas* camera::SetCanvas(canvas* Canvas);
            friend screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent);
            friend screenObject::~screenObject();
            friend void Internal::transformUI::SetParent(transformUI* ParentToSet);

            public:
            bool Enabled = true;

            camera* GetCamera(){return Camera;}

            virtual void OnCanvasSet(); // runs when the Canvas is set into a Camera
            virtual void OnCanvasUnSet();// runs when a Camera running this Canvas runs another one
            virtual void OnCanvasLoad();// runs when runs when the Canvas gets set in a Camera when it previously had no Camera
            virtual void OnCanvasUnLoad(); // runs when a Canvas loses the Camera running it

            std::vector<screenObject*> GetScreenObjects(){return ScreenObjects;}
            std::vector<screenObject*> GetParentlessScreenObjects(){return ParentlessScreenObjects;}
            std::vector<screenObject*> GetActiveScreenObjects(){return ActiveScreenObjects;}
            std::vector<screenObject*> GetInactiveScreenObjects(){return InctiveScreenObjects;}

            void PushActiveScreenObjects(screenObject* scren){ActiveScreenObjects.push_back(scren);}
            void PushInactiveScreenObjects(screenObject* scren){InctiveScreenObjects.push_back(scren);}
            void ClearActiveScreenObjects(){ActiveScreenObjects.clear();}
            void ClearInactiveScreenObjects(){InctiveScreenObjects.clear();}

            canvas(){}
            ~canvas();
        };

        canvas* CreateCanvas();

    }//UI
}//obj