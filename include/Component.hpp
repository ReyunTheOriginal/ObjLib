#pragma once

#include "GlobalTypes.hpp"
#include "Math.hpp"

struct SDL_Renderer;

namespace obj{
    struct gameObject;
    struct window;

    namespace Internal{
        struct renderSorter;
    }
        
    struct component{
        private:
        gameObject* GameObject = nullptr;

        public:
        bool Enabled = true;
        Internal::renderSorter* RenderLayer = nullptr;
        bool DidInit = false;

        gameObject* GetGameObject(){return GameObject;}

        virtual void EarlyRun() {}
        virtual void Run() {} //any updating that needs to be done
        virtual void LateRun() {}

        virtual void Draw(window* Window) {} //any rendering
        virtual void DebugDraw(window* Window) {} //any rendering in the window debug mode
        virtual void Init() {} //any init that needs to be ran once
        virtual void OnDestroy(){}

        void GStart(gameObject* obj){
            GameObject = obj;
        }

        ~component(){
            OnDestroy();
            
            if (RenderLayer)
                delete RenderLayer;
        }; //destructor
    };
}