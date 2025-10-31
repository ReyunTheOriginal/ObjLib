#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GlobalVar.hpp"
#include "Math.hpp"
#include "Input.hpp"
#include "HelperFunctions.hpp"

namespace obl{

    struct gameObject;

    struct color{
        int red=0, green=0,blue=0,alpha=255;

        SDL_Color ToSDLColor(){
            SDL_Color New;
            New.r = red; New.g = green; New.b = blue; New.a = alpha;
            return New;
        }
    };

    struct camera{
        private:
        float Zoom = 1;
        public:
        vector2 Position = {0,0};
        float Rotation = 0;

        void ZoomIn(float value){Zoom = std::max((double)(Zoom - value), 0.0001);}
        void ZoomOut(float value){Zoom = std::max((double)(Zoom + value), 0.0001);}
        float GetZoom(){ return Zoom;}

        camera(){};
    };

    struct window{
        private:
        std::string Title = "Hello World";
        std::string DebugTitle = "";

        std::string DebugifyTitle(std::string New);
            
        public:
        SDL_Window* SDLWindow;
        SDL_Renderer* SDLRenderer;

        camera* ActiveCamera = nullptr;
        Internal::Cursor* Cursor = nullptr;

        bool Debug = false;

        bool ClearFrame = true;
        color ClearColor = {0,0,0,255};

        std::vector<std::shared_ptr<gameObject>> WindowObjects;
        std::vector<gameObject*> RenderableObjects;
        std::vector<camera*> Cameras;

        vector2 GetResulotion();
        void SetResulotion(vector2 newRes){SDL_SetWindowSize(SDLWindow, newRes.x, newRes.y);}
        std::string GetTitle(){return Title;}
        void SetTitle(std::string newTitle);

        void Update();

        vector2 ScreenToWorldPosition(const vector2& ScreenPos);
        vector2 WorldToScreenPosition(const vector2& WorldPos);
        
        ~window(){
            for (auto* cam : Cameras){delete cam;}
            if (Cursor)delete Cursor;
            if (SDLRenderer) SDL_DestroyRenderer(SDLRenderer);
            if (SDLWindow) SDL_DestroyWindow(SDLWindow);
            RenderableObjects.clear();
        }
    };

}