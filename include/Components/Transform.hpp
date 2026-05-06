#pragma once

#include "Component.hpp"
#include <unordered_set>

namespace obj{
    struct gameObject;

    namespace Internal{
        struct transform : component{
            private:
            gameObject* GameObject = nullptr;
            transform* Parent = nullptr;
            std::unordered_set<transform*> Children;

            public:
            vector2 Position = {0,0};
            vector2 Scale = {1,1};
            float Rotation = 0;

            gameObject* GetGameObject(){return GameObject;}
            transform* GetParent(){return Parent;}
            std::unordered_set<transform*> GetChildren(){return Children;}

            void SetParent(transform* ParentToSet);

            transform(gameObject* gameObj){
                GameObject = gameObj;
            }

            ~transform();
        };
    }
}