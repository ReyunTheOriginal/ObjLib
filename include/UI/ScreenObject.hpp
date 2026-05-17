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

            std::unordered_set<std::string> Tags;
            public:
            std::string Name = "new GameObject";
            Internal::transformUI* UITransform;

            bool Enabled = true;
            
            int GetID(){return ID;}
            canvas* GetCanvas(){return Canvas;}

            std::unordered_map<std::type_index, screenComponent*> Components;

            screenComponent AddComponent();

            std::string AddTag(std::string newTag){
                if (!Tags.contains(newTag)){
                    Tags.insert(newTag);
                }

                return newTag;
            }

            void RemoveTag(std::string Tag){
                if (Tags.contains(Tag)){
                    Tags.erase(Tag);
                }
            }

            bool HasTag(std::string Tag){
                return Tags.contains(Tag);
            }

            template<typename T>
            T* GetComponent(){
                auto ref = Components.find(typeid(T));
                if (ref != Components.end()) return static_cast<T*>(ref->second);
                return nullptr;
            }

            template<typename T>
            T* AddComponent(){
                // 1. Check if a component of this type already exists
                auto ref = Components.find(typeid(T));
                if (ref != Components.end()) {
                    delete ref->second; // Delete the old component first!
                    Components.erase(ref);
                }

                // 2. Now allocate and insert the new one
                auto comp = new T();
                comp->CStart(this);

                Components[std::type_index(typeid(T))] = comp;
                return comp;
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