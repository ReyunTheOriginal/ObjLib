#pragma once

#include "Math.hpp"
#include "UI/ScreenComponent.hpp"
#include <vector>

#include <typeinfo>
#include "GlobalTypes.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <typeindex>
#include <cctype>

struct SDL_Renderer;

namespace obj{

    namespace Internal{
        struct renderSorter;
    }

    namespace UI{
        struct canvas;

        namespace Internal{
            struct transformUI;
        }

        struct screenObject{
            private:
            canvas* Canvas = nullptr;
            int ID = 0;

            friend screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent);

            public:
            std::string Name = "new GameObject";
            Internal::transformUI* UITransform;

            bool Enabled = true;
            
            int GetID(){return ID;}
            canvas* GetCanvas(){return Canvas;}

            std::unordered_map<std::type_index, screenComponent*> Components;

            screenComponent AddComponent();

            template<typename T>
            T* GetComponent(){
                auto ref = Components.find(typeid(T));
                if (ref != Components.end()) return static_cast<T*>(ref->second);
                return nullptr;
            }

            template<typename T>
            T* AddComponent(){
                auto comp = new T();
                comp->SStart(this);
                comp->RenderLayer = new ::obj::Internal::renderSorter();

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

        screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent = nullptr);
    }//UI
}//obj