#include "UI/Canvas.hpp"

#include "Camera.hpp"
#include "UI/ScreenObject.hpp"

namespace obj{

    namespace UI{

        canvas* CreateCanvas(camera* Camera){
            if (!Camera) return nullptr;

            canvas* newCanvas = new canvas();
            newCanvas->Camera = Camera;
            Camera->ActiveCanvas = newCanvas;

            return newCanvas;
        }

        canvas::~canvas(){
            auto copy = ScreenObjects;
            for (auto& obj : copy){
                if (obj) delete obj;
            }
            copy.clear();
            ScreenObjects.clear();
        }


    }//UI
}//obj