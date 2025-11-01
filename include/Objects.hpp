#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include "Math.hpp"
#include "string"
#include "GlobalTypes.hpp"

namespace obl{

    struct gameObject;
    struct window;

    namespace Internal{
        struct component {
            gameObject* GameObject = nullptr;
            bool Enabled = true;

            virtual ~component() = default;
            virtual void Run() {}
            virtual void Draw() {}
            virtual void Init() {}
        };
        struct transform;
    }

    struct gameObject{
        private:
        static unsigned int NextID;
        int ID;

        public:
        std::string Name;
        std::unique_ptr<Internal::transform> Transform = nullptr;

        bool Activated = true;

        std::unordered_map<std::type_index, std::unique_ptr<Internal::component>> Components;

        template<typename T>
        T* GetComponent(){
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()) return static_cast<T*>(ref->second.get());
            return nullptr;
        }

        template<typename T>
         T* GetConstComponent() const{
            auto ref = Components.find(typeid(T));
            if (ref != Components.end()) return static_cast<T*>(ref->second.get());
            return nullptr;
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            comp->GameObject = this;
            comp->Init();

            auto [it, inserted] = Components.emplace(std::type_index(typeid(T)), std::move(comp));
            return *static_cast<T*>(it->second.get());
        }

        template<typename T>
        void RemoveComponent(){
            auto* com = GetComponent<T>();
            if (com)
                Components.erase(std::type_index(typeid(T)));
        }

        int GetID()const{return ID;}

        gameObject(){
            ID = NextID;
            NextID++;
        }

        ~gameObject(){
            for (auto& com : Components)delete com.second.get();
            Components.clear();
        }

        bool operator==(const gameObject& other)const{
            return this->ID == other.GetID();
        }
    };

    namespace Internal{
        struct transform{
            private:
            gameObject* Parent = nullptr;
            gameObject* GameObject = nullptr;

            friend struct window;

            public:
            vector2 Position = {0,0};
            vector2 LocalPosition = {0,0};

            float Rotation = 0.0;
            float LocalRotation = 0.0;

            float Size = 1;
            float LocalSize = 1;

            std::unordered_map<int, gameObject*> Children;

            window* Window = nullptr;

            gameObject* SetParent(gameObject* parent);
            gameObject* SetChild(gameObject* child);

            gameObject* GetParent();
            gameObject* GetChild(int childID);

            vector2 WorldToLocal(vector2 WorldPos);
            vector2 LocalToWorld(vector2 LocalPos);

            void Init(gameObject* obj){GameObject = obj;}

            transform(float x = 0, float y = 0){Position = {x, y};}
        };
    }

}