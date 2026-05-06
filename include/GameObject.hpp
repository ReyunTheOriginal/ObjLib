#pragma once

#include <typeinfo>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

        friend gameObject* CreateGameObject(scene* Scene, Internal::transform* Parent);
        scene* Scene = nullptr;

        std::unordered_set<std::string> Tags;

        public:
        std::string Name = "new GameObject";
        Internal::transform* Transform = nullptr;

        bool Enabled = true;
        
        std::unordered_map<std::type_index, Internal::component*> Components;

        Internal::component* AddComponent();

        int GetID(){return ID;}

        scene* GetScene(){return Scene;}
        void SetScene(scene* SceneToSendTo);

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

    gameObject* CreateGameObject(scene* Scene, Internal::transform* Parent = nullptr);
}