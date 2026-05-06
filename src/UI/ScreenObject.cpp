#include "UI/ScreenObject.hpp"
#include <algorithm>

#include "GlobalLists.hpp"
#include "UI/Canvas.hpp"
#include "Scene.hpp"

namespace obj{

    namespace UI{
        void Internal::transformUI::SetParent(transformUI* ParentToSet){
            if (Parent){
                if (Parent->Children.contains(this)){
                    Parent->Children.erase(this);
                }
            }

            if (ParentToSet == nullptr)
                ParentToSet = ScreenObject->GetCanvas()->ObjectParent;

            if (!ParentToSet->Children.contains(this)){
                ParentToSet->Children.insert(this);
            }

            Parent = ParentToSet;
        }

        Internal::transformUI::~transformUI(){
            if (ScreenObject)delete ScreenObject;

            for (transformUI* child : Children){
                if (child)delete child;
            }
        }

        screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent){
            if (!Canvas)return nullptr;

            screenObject* newObj = new screenObject();
            newObj->Canvas = Canvas;
            Canvas->UI.push_back(newObj);

            newObj->UITransform = new Internal::transformUI(newObj);

            if (Parent == nullptr){
                newObj->UITransform->SetParent(Canvas->ObjectParent);
            }else{
                newObj->UITransform->SetParent(Parent);
            }

            ::obj::Internal::GlobalScreenObjects.push_back(newObj);
            newObj->ID = ::obj::Internal::Obj_ID;
            newObj->Name = "screenObject #" + std::to_string(::obj::Internal::Obj_ID);
            ::obj::Internal::Obj_ID++;
            return newObj;
        }

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

            if (UITransform)delete UITransform;
            Components.clear();
        }
    }
}