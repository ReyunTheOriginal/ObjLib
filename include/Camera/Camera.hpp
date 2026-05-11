#pragma once

#include "Math.hpp"
#include "GlobalTypes.hpp"
#include "Window.hpp"
#include "Camera/CameraComponent.hpp"

#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cctype>

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

        std::unordered_map<std::type_index, cameraComponent*> Components;

        UI::canvas* GetCanvas(){return ActiveCanvas;}
        UI::canvas* SetCanvas(UI::canvas* Canvas);

        virtual void OnCanvasChange(); // runs when SetCanvas() runs with a new Canvas

        window* GetWindow(){return ActiveWindow;}

        vector2 GetResolution(){return Resolution;}
        vector2 SetResolution(const vector2& res);

        vector2 ScreenToWorldPosition(const vector2& PosToTranslate);
        vector2 WorldToScreenPosition(const vector2& PosToTranslate);

        template<typename T>
        T* GetComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()) return static_cast<T*>(ref->second);
            return nullptr;
        }

        template<typename T>
        T* AddComponent(){
            auto comp = new T();
            comp->CStart(this);

            auto [it, inserted] = Components.emplace(std::type_index(typeid(T)), comp);
            return static_cast<T*>(it->second);
        }
        template<typename T>
        void DestroyComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()){
                Components.erase(ref);
                delete ref->second;
            }
        }

        camera(){SetResolution(Resolution);};
    };

    camera* CreateCamera(window* Window);
    
}