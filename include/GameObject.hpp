#pragma once

#include <typeinfo>
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cctype>

#include "Component.hpp"

namespace obj{
    struct gameObject;
    struct scene;

    namespace Internal{
        struct transform;
        struct renderSorter;
    }

    struct gameObject{
        private:
        int ID = 0;

        friend gameObject* CreateGameObject(scene* Scene);
        scene* Scene = nullptr;

        public:
        std::string Name = "new GameObject";
        Internal::transform* Transform = nullptr;
        
        std::unordered_map<std::type_index, Internal::component*> Components;

        Internal::component* AddComponent();

        int GetID(){return ID;}

        scene* GetScene(){return Scene;}
        void SetScene(scene* SceneToSendTo);

        template<typename T>
        T* GetComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()) return static_cast<T*>(ref->second);
            return nullptr;
        }

        template<typename T>
        T* AddComponent(){
            auto comp = new T();
            comp->RenderLayer = new Internal::renderSorter();
            comp->GameObject = this;

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

        ~gameObject();
    };

    gameObject* CreateGameObject(scene* Scene);
}