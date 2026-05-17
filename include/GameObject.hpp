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
        
        std::unordered_map<std::type_index, component*> Components;

        component* AddComponent();

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
        void DestroyComponent(){
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