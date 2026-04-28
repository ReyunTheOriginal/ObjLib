#pragma once

#include "Math.hpp"
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cctype>
#include <typeinfo>
#include <string>
#include "GlobalLists.hpp"
#include "Components.hpp"

namespace obj{
    struct scene;

    struct gameObject{
        private:
        int ID = 0;

        friend gameObject* CreateGameObject(scene* Scene);

        public:
        std::string Name = "new GameObject";
        scene* Scene = nullptr;
        vector2 Position = {0,0};
        vector2 Size = {1,1};
        float Rotation = 0;
        
        std::unordered_map<std::type_index, Internal::component*> Components;

        Internal::component* AddComponent();

        int GetID(){return ID;}

        template<typename T>
        T* GetComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()) return static_cast<T*>(ref->second);
            return nullptr;
        }

        template<typename T>
        T* AddComponent(){
            auto comp = new T();
            comp->GameObject = this;

            auto [it, inserted] = Components.emplace(std::type_index(typeid(T)), comp);
            return static_cast<T*>(it->second);
        }
        template<typename T>
        void RemoveComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()){
                ref->second->Destroy();
                delete ref->second;
                Components.erase(ref);
            }
        }

        ~gameObject(){
            for(auto& com : Components){
                com.second->Destroy();
            }
        }
    };

    gameObject* CreateGameObject(scene* Scene);
    void DestroyGameObject(gameObject*);
}