#pragma once

#include "Math.hpp"
#include <vector>

#include "GlobalTypes.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace obj{
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

                vector2 GetScreenPosition(){return Position;}
                vector2 GetScreenScale(){return Scale;}
                float GetScreenRotation(){return Rotation;}

                void SetScreenPositionRaw(vector2 position){Position = position;}
                void SetScreenScaleRaw(vector2 scale){Scale = scale;}
                void SetScreenRotationRaw(float rotation){Rotation = rotation;}

                void SetScreenPosition(vector2 Pos);
                void SetScreenScale(vector2 Scale);
                void SetScreenRotation(float Rotation);

                vector2 ScreenToLocal(vector2 ScreenPos);
                vector2 LocalToScreen(vector2 LocalPos);

                void SetParent(transformUI* ParentToSet);

                bool IsDescendantOf(transformUI* target){
                    transformUI* current = Parent;

                    while (current != nullptr){
                        if (current == target)
                            return true;

                        current = current->Parent;
                    }

                    return false;
                }

                transformUI(screenObject* screenObj){
                    ScreenObject = screenObj;
                }

                ~transformUI();

            };
        }
    }
}