#include "UI/Canvas.hpp"

#include "Camera/Camera.hpp"
#include "UI/ScreenObject.hpp"

namespace obj{

    namespace UI{

        void canvas::OnCanvasSet(){}
        void canvas::OnCanvasUnSet(){}
        void canvas::OnCanvasLoad(){}
        void canvas::OnCanvasUnLoad(){}

        canvas* CreateCanvas(){
            return new canvas();
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