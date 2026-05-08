#pragma once

#include <vector>
#include "GlobalTypes.hpp"

struct SDL_Renderer;

namespace obj{
    struct window;

    namespace UI{
        struct screenObject;

        namespace Internal{
            struct transformUI;
        }

        struct screenComponent{
            screenObject* ScreenObject = nullptr;

            ::obj::Internal::renderSorter* RenderLayer = nullptr;

            bool Enabled = true;
            bool DidInit = false;

            virtual void Run() {} //any updating that needs to be done
            virtual void Draw(window* Window) {} //any rendering
            virtual void DebugDraw(window* Window) {} //any rendering in the window debug mode
            virtual void Init() {} //any init that needs to be ran once
            virtual void OnDestroy(){}
            virtual ~screenComponent(){
                OnDestroy();
                
                if (RenderLayer)
                    delete RenderLayer;
            } //destructor

        };

    }

}