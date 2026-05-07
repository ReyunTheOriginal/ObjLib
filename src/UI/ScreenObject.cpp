#include "UI/ScreenObject.hpp"
#include <algorithm>

#include "GlobalLists.hpp"
#include "UI/Canvas.hpp"
#include "Scene.hpp"
#include "Math.hpp"

namespace obj{

    namespace UI{
        vector2 Internal::transformUI::WorldToLocal(vector2 WorldPos){
            if (!Parent) return WorldPos;
            vector2 Local;

            Local = WorldPos - Parent->Position;

            float rad = Math::Deg2Rad(-Parent->Rotation);
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);

            Local = { Local.x * cs - Local.y * sn, Local.x * sn + Local.y * cs };
            Local.x /= Parent->Scale.x;
            Local.y /= Parent->Scale.y;
            return Local;
        }

        vector2 Internal::transformUI::LocalToWorld(vector2 LocalPos){
            // If no parent, LocalPos IS the world position
            if (!Parent) return LocalPos;
            
            // Transform local to world: rotate, scale, then add parent's world position
            vector2 World = LocalPos;
            
            // Apply parent's rotation to this position
            float rad = Math::Deg2Rad(Parent->Rotation);
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);
            World = { World.x * cs - World.y * sn, World.x * sn + World.y * cs };
            
            // Scale by parent's scale (to account for parent's coordinate system)
            World.x *= Parent->Scale.x;
            World.y *= Parent->Scale.y;
            
            // Add parent's world position
            return World + Parent->Position;
        }

        void Internal::transformUI::SetWorldPosition(vector2 Pos){
            LocalPosition = WorldToLocal(Pos);
            // Immediately update world position
            Position = LocalToWorld(LocalPosition);
        }

        void Internal::transformUI::SetWorldScale(vector2 Scale){
            // Convert world scale to local scale
            if (Parent) {
                LocalScale.x = Scale.x / Parent->Scale.x;
                LocalScale.y = Scale.y / Parent->Scale.y;
            } else {
                LocalScale = Scale;
            }
            // Update world scale immediately
            this->Scale = Parent ? 
            Parent->Scale * LocalScale : 
            LocalScale;
        }

        void Internal::transformUI::SetWorldRotation(float Rotation){
            // Convert world rotation to local rotation
            if (Parent) {
                LocalRotation = Rotation - Parent->Rotation;
            } else {
                LocalRotation = Rotation;
            }
            // Update world rotation immediately
            this->Rotation = Parent ? 
            Parent->Rotation + LocalRotation : 
            LocalRotation;
        }

        void Internal::transformUI::SetParent(transformUI* ParentToSet){
            if (Parent){
                if (Parent->Children.contains(this)){
                    Parent->Children.erase(this);
                }
            }

            if (ParentToSet && !ParentToSet->Children.contains(this)){
                ParentToSet->Children.insert(this);
            }

            if (ParentToSet == nullptr){
                ScreenObject->GetCanvas()->ParentlessScreenObjects.push_back(ScreenObject);
            }

            LocalPosition = ParentToSet ? WorldToLocal(Position) : Position;
            LocalRotation = ParentToSet ? Rotation - ParentToSet->Rotation : Rotation;
            LocalScale.x = ParentToSet ? Scale.x / ParentToSet->Scale.x : Scale.x;
            LocalScale.y = ParentToSet ? Scale.y / ParentToSet->Scale.y : Scale.y;

            Parent = ParentToSet;
        }

        Internal::transformUI::~transformUI(){

            // Clear parent reference
            if (Parent && Parent->Children.contains(this)){
                Parent->Children.erase(this);
            }

            if (!Parent){
                std::erase(ScreenObject->GetCanvas()->ParentlessScreenObjects, ScreenObject);
            }

            if (ScreenObject)delete ScreenObject;

            for (transformUI* child : Children){
                if (child)delete child;
            }

            Children.clear();
            Children.clear();
        }

        screenObject* CreateScreenObject(canvas* Canvas, Internal::transformUI* Parent){
            if (!Canvas)return nullptr;

            screenObject* newObj = new screenObject();
            newObj->Canvas = Canvas;
            Canvas->ScreenObjects.push_back(newObj);

            newObj->UITransform = new Internal::transformUI(newObj);

            newObj->UITransform->SetParent(Parent);

            // Update world position, rotation, and scale based on parent
            vector2 worldPos = newObj->UITransform->LocalPosition;
            if (newObj->UITransform->GetParent()){
                // Rotate by parent's rotation
                float rad = Math::Deg2Rad(newObj->UITransform->GetParent()->GetWorldRotation());
                float cs = Math::Cos(rad);
                float sn = Math::Sin(rad);
                worldPos = { worldPos.x * cs - worldPos.y * sn, worldPos.x * sn + worldPos.y * cs };
                
                // Scale by parent's scale
                worldPos.x *= newObj->UITransform->GetParent()->GetWorldScale().x;
                worldPos.y *= newObj->UITransform->GetParent()->GetWorldScale().y;
                
                // Add parent's position
                worldPos += newObj->UITransform->GetParent()->GetWorldPosition();
            }
            newObj->UITransform->SetWorldPositionRaw(worldPos);
            
            newObj->UITransform->SetWorldRotationRaw(newObj->UITransform->GetParent() ? 
                newObj->UITransform->GetParent()->GetWorldRotation() + newObj->UITransform->LocalRotation : 
                newObj->UITransform->LocalRotation);
            
            newObj->UITransform->SetWorldScaleRaw(newObj->UITransform->GetParent() ? 
                newObj->UITransform->GetParent()->GetWorldScale() * newObj->UITransform->LocalScale : 
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

            if (UITransform)delete UITransform;
            Components.clear();
        }
    }
}