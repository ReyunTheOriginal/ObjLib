#pragma once

#include "Math.hpp"
#include "UI/ScreenComponent.hpp"
#include <vector>

#include <typeinfo>
#include "GlobalTypes.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cctype>

struct SDL_Renderer;

namespace obj{

    namespace Internal{
        struct renderSorter;
    }

    namespace UI{

        namespace Internal{
            struct transformUI{
                vector2 Position = {0,0};
                vector2 Scale = {1,1};
                float Rotation = 0;

                std::vector<transformUI*> Children;
            };
        }

        struct canvas;

        struct screenObject{
            private:
            canvas* Canvas = nullptr;
            int ID = 0;

            friend screenObject* CreateScreenObject(canvas* Canvas);

            public:
            std::string Name = "new GameObject";
            Internal::transformUI* UITransform;
            
            int GetID(){return ID;}
            canvas* GetCanvas(){return Canvas;}

            std::unordered_map<std::type_index, Internal::screenComponent*> Components;

            Internal::screenComponent AddComponent();

            template<typename T>
            T* GetComponent(){
                auto ref = Components.find(typeid(T));
                if (ref != Components.end()) return static_cast<T*>(ref->second);
                return nullptr;
            }

            template<typename T>
            T* AddComponent(){
                auto comp = new T();
                comp->RenderLayer = new ::obj::Internal::renderSorter();
                comp->ScreenObject = this;

                auto [it, inserted] = Components.emplace(std::type_index(typeid(T)), comp);
                return static_cast<T*>(it->second);
            }
            template<typename T>
            void RemoveComponent(){
                auto ref = Components.find(typeid(T));
                if (ref != Components.end()){
                    Components.erase(ref);
                    delete ref->second;
                }
            }

            ~screenObject();

        };

        screenObject* CreateScreenObject(canvas* Canvas);
    }//UI
}//obj