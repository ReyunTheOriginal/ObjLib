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
        struct screenObject;
        struct canvas;

        namespace Internal{
            struct transformUI{
                private:
                screenObject* ScreenObject = nullptr;
                transformUI* Parent = nullptr;
                std::unordered_set<transformUI*> Children;

                vector2 Position = {0,0};
                vector2 Scale = {1,1};
                float Rotation = 0;

                public:
                vector2 LocalPosition = {0,0};
                vector2 LocalScale = {1,1};
                float LocalRotation = 0;

                screenObject* GetScreenObject(){return ScreenObject;}
                transformUI* GetParent(){return Parent;}
                std::unordered_set<transformUI*> GetChildren(){return Children;}

                vector2 GetWorldPosition(){return Position;}
                vector2 GetWorldScale(){return Scale;}
                float GetWorldRotation(){return Rotation;}

                void SetWorldPositionRaw(vector2 position){Position = position;}
                void SetWorldScaleRaw(vector2 scale){Scale = scale;}
                void SetWorldRotationRaw(float rotation){Rotation = rotation;}

                void SetWorldPosition(vector2 Pos);
                void SetWorldScale(vector2 Scale);
                void SetWorldRotation(float Rotation);

                vector2 WorldToLocal(vector2 WorldPos);
                vector2 LocalToWorld(vector2 LocalPos);

                void SetParent(transformUI* ParentToSet);

                transformUI(screenObject* screenObj){
                    ScreenObject = screenObj;
                }

                ~transformUI();

            };
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

        screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent = nullptr);
    }//UI
}//obj