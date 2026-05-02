#pragma once

#include "Math.hpp"
#include "GlobalTypes.hpp"

namespace obj{
    struct scene;
    struct window;
    
    namespace UI{
        struct canvas;
    }

    struct camera{
        private:
        vector2 Resolution = {800, 600};

        public:
        vector2 Position = {0,0};
        float Rotation = 0;
        float Zoom = 1;

        window* ActiveWindow = nullptr;
        UI::canvas* ActiveCanvas = nullptr;

        vector2 GetResolution(){return Resolution;}
        vector2 SetResolution(const vector2& res);

        vector2 ScreenToWorldPosition(const vector2& PosToTranslate);
        vector2 WorldToScreenPosition(const vector2& PosToTranslate);
        camera(){SetResolution(Resolution);};
    };

    camera* CreateCamera(window* Window);
    void DestroyCamera(camera* Camera);
    
}