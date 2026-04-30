#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <memory>

#include "Math.hpp"
#include "GlobalLists.hpp"
#include "Scene.hpp"

namespace obj{
    
    struct window{
        private:
        scene* Scene = nullptr;
        int ID = 0;
        vector2 CachedResolution = {0, 0};

        friend window* CreateWindow(std::string title, obj::vector2 resolution, Uint64 WindowFlags);
        public:
        
        SDL_Window* SDLwindow = nullptr;
        SDL_Renderer* SDLrenderer = nullptr;
        int FPS = 0;
        bool Debug = false;

        int GetID(){return ID;}

        void SetScene(scene* SceneToSet);
        scene* GetScene(){return Scene;}
        
        void SetTitle(std::string NewTitle){SDL_SetWindowTitle(SDLwindow, NewTitle.c_str());}
        std::string GetTitle(){return SDL_GetWindowTitle(SDLwindow);}

        vector2 GetResolution(){return CachedResolution;}
        vector2 SetResolution(const vector2& res);

        window();
        ~window(){
            std::erase(Internal::GlobalWindows, this);
            
            if (Scene){
                std::erase(Scene->Windows, this);
            }

            if (SDLrenderer)SDL_DestroyRenderer(this->SDLrenderer);
            if (SDLwindow)SDL_DestroyWindow(this->SDLwindow);
        };
    };

    window* CreateWindow(std::string title,obj::vector2 resolution,Uint64 WindowFlags = SDL_WINDOW_RESIZABLE);
}