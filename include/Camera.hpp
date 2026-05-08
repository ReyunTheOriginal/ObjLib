#pragma once

#include "Math.hpp"
#include "GlobalTypes.hpp"
#include "Window.hpp"

namespace obj{
    struct scene;
    
    namespace UI{
        struct canvas;
    }

    struct camera{
        private:
        vector2 Resolution = {800, 600};

        window* ActiveWindow = nullptr;
        UI::canvas* ActiveCanvas = nullptr;

        friend camera* CreateCamera(window* Window);
        friend camera* window::SetCamera(camera* Camera);
        friend window* CreateWindow(std::string title, obj::vector2 resolution);

        public:
        vector2 Position = {0,0};
        float Rotation = 0;
        float Zoom = 1;

        UI::canvas* GetCanvas(){return ActiveCanvas;}
        UI::canvas* SetCanvas(UI::canvas* Canvas);

        window* GetWindow(){return ActiveWindow;}

        vector2 GetResolution(){return Resolution;}
        vector2 SetResolution(const vector2& res);

        vector2 ScreenToWorldPosition(const vector2& PosToTranslate);
        vector2 WorldToScreenPosition(const vector2& PosToTranslate);
        camera(){SetResolution(Resolution);};
    };

    camera* CreateCamera(window* Window);
    
}