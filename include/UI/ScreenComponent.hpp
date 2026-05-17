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
            private:
            screenObject* ScreenObject = nullptr;

            public:
            ::obj::Internal::renderSorter* RenderLayer = nullptr;

            bool Enabled = true;
            bool DidInit = false;

            screenObject* GetScreenObject(){return ScreenObject;}

            virtual void EarlyRun() {}
            virtual void Run() {} //any updating that needs to be done
            virtual void LateRun() {}

            virtual void Draw(window* Window) {} //any rendering
            virtual void DebugDraw(window* Window) {} //any rendering in the window debug mode
            virtual void Init() {} //any init that needs to be ran once
            virtual void OnDestroy(){}

            void SStart(screenObject* scren){
                ScreenObject = scren;
            }

            ~screenComponent(){
                OnDestroy();
                if (RenderLayer)
                    delete RenderLayer;
            } //destructor

        };

    }

}