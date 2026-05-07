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

            vector2 Position = {0,0};
            vector2 Scale = {1,1};
            float Rotation = 0;

            public:
            vector2 LocalPosition = {0,0};
            vector2 LocalScale = {1,1};
            float LocalRotation = 0;

            gameObject* GetGameObject(){return GameObject;}
            transform* GetParent(){return Parent;}
            std::unordered_set<transform*> GetChildren(){return Children;}

            void SetParent(transform* ParentToSet);

            vector2 GetWorldPosition(){return Position;}
            vector2 GetWorldScale(){return Scale;}
            float GetWorldRotation(){return Rotation;}

            void SetWorldPosition(vector2 Pos);
            void SetWorldScale(vector2 Scale);
            void SetWorldRotation(float Rotation);

            void SetWorldPositionRaw(vector2 position){Position = position;}
            void SetWorldScaleRaw(vector2 scale){Scale = scale;}
            void SetWorldRotationRaw(float rotation){Rotation = rotation;}

            vector2 WorldToLocal(vector2 WorldPos);
            vector2 LocalToWorld(vector2 LocalPos);

            transform(gameObject* gameObj){
                GameObject = gameObj;
            }

            ~transform();
        };
    }
}