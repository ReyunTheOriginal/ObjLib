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
        std::string Title = "obj::Hello, World!";
        vector2 Resolution = vector2(800, 600);
        scene* Scene = nullptr;
        public:
        
        SDL_Window* SDLwindow = nullptr;
        SDL_Renderer* SDLrenderer = nullptr;
        int FPS = 0;
        bool Debug = false;

        void SetScene(scene* SceneToSet){Scene = SceneToSet;SceneToSet->Window = this;}
        scene* GetScene(){return Scene;}

        window();
        ~window(){
            if (SDLrenderer)SDL_DestroyRenderer(this->SDLrenderer);
            if (SDLwindow)SDL_DestroyWindow(this->SDLwindow);
        };
    };

    window* CreateWindow(std::string title,obj::vector2 resolution,Uint64 WindowFlags = SDL_WINDOW_RESIZABLE);
    void DestroyWindow(window* Window);
}