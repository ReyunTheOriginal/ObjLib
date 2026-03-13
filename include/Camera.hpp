#pragma once

#include "Math.hpp"
#include "GlobalTypes.hpp"
#include "Window.hpp"

namespace obj{    
    struct camera{
        private:
        vector2 Resolution = {800, 600};

        public:
        vector2 Position = {0,0};
        float Rotation = 0;
        float Zoom = 1;
        color ColorTint = {255,0,0,0};
        scene* ActiveScene = nullptr;
        window* ActiveWindow = nullptr;

        vector2 GetResolution(){return Resolution;}
        vector2 SetResolution(const vector2& res);

        vector2 ScreenToWorldPosition(const vector2& PosToTranslate);
        vector2 WorldToScreenPosition(const vector2& PosToTranslate);
        camera(){if (ActiveScene) SetResolution(Resolution);};
    };

    camera* CreateCamera();
    void DestroyCamera(camera* Camera);
    
}