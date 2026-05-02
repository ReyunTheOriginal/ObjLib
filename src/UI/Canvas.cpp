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
            auto copy = UI;
            for (auto& obj : copy){
                if (obj) delete obj;
            }
            UI.clear();
        }


    }//UI
}//obj