#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <memory>

#include "Math.hpp"
#include "GlobalLists.hpp"

namespace obj{
    struct scene;
    struct camera;
    
    struct window{
        private:
        scene* Scene = nullptr;
        int ID = 0;
        vector2 CachedResolution = {0, 0};
        std::string CachedTitle = "";

        SDL_Window* SDLwindow = nullptr;
        SDL_Renderer* SDLrenderer = nullptr;
        camera* ActiveCamera = nullptr;

        friend window* CreateWindow(std::string title, obj::vector2 resolution);
        public:
        
        bool Debug = false;

        int GetID(){return ID;}

        camera* SetCamera(camera* Camera);
        camera* GetCamera(){return ActiveCamera;}

        SDL_Renderer* GetSDLRenderer(){return SDLrenderer;}
        SDL_Window* GetSDLWindow(){return SDLwindow;}

        scene* SetScene(scene* SceneToSet);
        scene* GetScene(){return Scene;}

        virtual void OnSceneChange(); // runs when SetScene() runs with a new Scene
        
        std::string SetTitle(std::string NewTitle);
        std::string GetTitle();

        vector2 GetResolution(){return CachedResolution;}
        vector2 SetResolution(const vector2& res);

        virtual void DebugDisplay();

        window();
        ~window();
    };

    window* CreateWindow(std::string title,obj::vector2 resolution);
}