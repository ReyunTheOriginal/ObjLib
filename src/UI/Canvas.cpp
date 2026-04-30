#include "UI/Canvas.hpp"

#include "Camera.hpp"

namespace obj{

    namespace UI{

        canvas* CreateCanvas(camera* Camera){
            canvas* newCanvas = new canvas();
            newCanvas->Camera = Camera;
            Camera->ActiveCanvas = newCanvas;

            return newCanvas;
        }

    }//UI
}//obj