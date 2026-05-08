#include "UI/ScreenObject.hpp"
#include <algorithm>

#include "GlobalLists.hpp"
#include "UI/Canvas.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "UI/UITransform.hpp"

namespace obj{

    namespace UI{
        screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent){
            if (!Canvas)return nullptr;

            screenObject* newObj = new screenObject();
            newObj->Canvas = Canvas;
            Canvas->ScreenObjects.push_back(newObj);

            newObj->UITransform = new Internal::transformUI(newObj);

            newObj->UITransform->SetParent(Parent);

            // Update screen position, rotation, and scale based on parent
            vector2 screenPos = newObj->UITransform->LocalPosition;
            if (newObj->UITransform->GetParent()){
                // Rotate by parent's rotation
                float rad = Math::Deg2Rad(newObj->UITransform->GetParent()->GetScreenRotation());
                float cs = Math::Cos(rad);
                float sn = Math::Sin(rad);
                screenPos = { screenPos.x * cs - screenPos.y * sn, screenPos.x * sn + screenPos.y * cs };
                
                // Scale by parent's scale
                screenPos.x *= newObj->UITransform->GetParent()->GetScreenScale().x;
                screenPos.y *= newObj->UITransform->GetParent()->GetScreenScale().y;
                
                // Add parent's position
                screenPos += newObj->UITransform->GetParent()->GetScreenPosition();
            }
            newObj->UITransform->SetScreenPositionRaw(screenPos);
            
            newObj->UITransform->SetScreenRotationRaw(newObj->UITransform->GetParent() ? 
                newObj->UITransform->GetParent()->GetScreenRotation() + newObj->UITransform->LocalRotation : 
                newObj->UITransform->LocalRotation);
            
            newObj->UITransform->SetScreenScaleRaw(newObj->UITransform->GetParent() ? 
                newObj->UITransform->GetParent()->GetScreenScale() * newObj->UITransform->LocalScale : 
                newObj->UITransform->LocalScale);

            ::obj::Internal::GlobalScreenObjects.push_back(newObj);
            newObj->ID = ::obj::Internal::Obj_ID;
            newObj->Name = "screenObject #" + std::to_string(::obj::Internal::Obj_ID);
            ::obj::Internal::Obj_ID++;
            return newObj;
        }

        screenObject::~screenObject(){
            // Remove from scene
            if (Canvas){
                std::erase(Canvas->ScreenObjects, this);
            }
            
            // Remove from global list
            std::erase(::obj::Internal::GlobalScreenObjects, this);

            for(auto& com : Components){
                if (com.second){
                    delete com.second;
                }
            }

            if (Canvas){
                std::erase(Canvas->ParentlessScreenObjects, this);
            }

            if (UITransform)delete UITransform;

            Components.clear();
        }
    }
}