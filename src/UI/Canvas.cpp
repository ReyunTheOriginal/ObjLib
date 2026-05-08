#include "UI/Canvas.hpp"

#include "Camera.hpp"
#include "UI/ScreenObject.hpp"

namespace obj{

    namespace UI{

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