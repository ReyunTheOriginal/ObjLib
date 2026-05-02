#include "UI/ScreenObject.hpp"
#include <algorithm>

#include "GlobalLists.hpp"
#include "UI/Canvas.hpp"
#include "Scene.hpp"

namespace obj{

    namespace UI{
        screenObject::~screenObject(){
            // Remove from scene
            if (Canvas){
                std::erase(Canvas->UI, this);
            }
            
            // Remove from global list
            std::erase(::obj::Internal::GlobalScreenObjects, this);

            for(auto& com : Components){
                if (com.second){
                    delete com.second;
                }
            }
            Components.clear();
        }

        screenObject* CreateScreenObject(canvas* Canvas){
            if (!Canvas)return nullptr;

            screenObject* newObj = new screenObject();
            newObj->Canvas = Canvas;
            Canvas->UI.push_back(newObj);

            newObj->UITransform = new Internal::transformUI();

            ::obj::Internal::GlobalScreenObjects.push_back(newObj);
            newObj->ID = ::obj::Internal::Obj_ID;
            newObj->Name = "screenObject #" + std::to_string(::obj::Internal::Obj_ID);
            ::obj::Internal::Obj_ID++;
            return newObj;
        }
    }
}