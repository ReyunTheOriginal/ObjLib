#pragma once

#include "GlobalTypes.hpp"
#include "Math.hpp"

struct SDL_Renderer;

namespace obj{
    struct gameObject;
    struct window;

    namespace Internal{
        struct renderSorter;
        
        struct component{
            gameObject* GameObject = nullptr;
            bool Enabled = true;
            Internal::renderSorter* RenderLayer;
            bool DidInit = false;

            virtual void Run() {} //any updating that needs to be done
            virtual void Draw(window* Window) {} //any rendering
            virtual void DebugDraw(window* Window) {} //any rendering in the window debug mode
            virtual void Init() {} //any init that needs to be ran once
            virtual void OnDestroy(){}
            virtual ~component(){
                OnDestroy();
                
                if (RenderLayer)
                    delete RenderLayer;
            }; //destructor
        };
    }
}