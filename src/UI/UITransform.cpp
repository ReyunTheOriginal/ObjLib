#include "UI/UITransform.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/Canvas.hpp"

#include <algorithm>

namespace obj{
    namespace UI{
        vector2 Internal::transformUI::ScreenToLocal(vector2 ScreenPos){
            if (!Parent) return ScreenPos;
            vector2 Local;

            Local = ScreenPos - Parent->Position;

            float rad = Math::Deg2Rad(Parent->Rotation);  // Negate removed for clockwise
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);

            Local = { Local.x * cs - Local.y * sn, Local.x * sn + Local.y * cs };
            Local.x /= Parent->Scale.x;
            Local.y /= Parent->Scale.y;
            return Local;
        }

        vector2 Internal::transformUI::LocalToScreen(vector2 LocalPos){
            // If no parent, LocalPos IS the Screen position
            if (!Parent) return LocalPos;
            
            // Transform local to Screen: rotate, scale, then add parent's Screen position
            vector2 Screen = LocalPos;
            
            // Apply parent's rotation to this position
            float rad = Math::Deg2Rad(-Parent->Rotation);  // Negate for clockwise
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);
            Screen = { Screen.x * cs - Screen.y * sn, Screen.x * sn + Screen.y * cs };
            
            // Scale by parent's scale (to account for parent's coordinate system)
            Screen.x *= Parent->Scale.x;
            Screen.y *= Parent->Scale.y;
            
            // Add parent's Screen position
            return Screen + Parent->Position;
        }

        void Internal::transformUI::SetScreenPosition(vector2 Pos){
            LocalPosition = ScreenToLocal(Pos);
            // Immediately update Screen position
            Position = LocalToScreen(LocalPosition);
        }

        void Internal::transformUI::SetScreenScale(vector2 Scale){
            // Convert Screen scale to local scale
            if (Parent) {
                LocalScale.x = Scale.x / Parent->Scale.x;
                LocalScale.y = Scale.y / Parent->Scale.y;
            } else {
                LocalScale = Scale;
            }
            // Update Screen scale immediately
            this->Scale = Parent ? 
            Parent->Scale * LocalScale : 
            LocalScale;
        }

        void Internal::transformUI::SetScreenRotation(float Rotation){
            // Convert Screen rotation to local rotation
            if (Parent) {
                LocalRotation = Rotation - Parent->Rotation;
            } else {
                LocalRotation = Rotation;
            }
            // Update Screen rotation immediately
            this->Rotation = Parent ? 
            Parent->Rotation + LocalRotation : 
            LocalRotation;
        }

        void Internal::transformUI::SetParent(transformUI* ParentToSet){
            transformUI* parentSnapShot = ParentToSet;

            // Capture screen position/rotation/scale BEFORE changing parent
            // Use LocalToScreen to compute actual screen position since Position may be stale
            vector2 ScreenPosition = LocalToScreen(LocalPosition);
            vector2 ScreenScale = Parent ? (Scale.x / Parent->Scale.x != 0 ? Scale : LocalScale * Parent->Scale) : Scale;
            float ScreenRotation = Parent ? (Rotation - Parent->Rotation + Parent->Rotation) : Rotation;

            std::erase(ScreenObject->GetCanvas()->ParentlessScreenObjects, ScreenObject);

            if ((parentSnapShot && parentSnapShot->IsDescendantOf(this)) || parentSnapShot == this)
                parentSnapShot = nullptr;

            if (Parent && Parent->Children.contains(this))
                Parent->Children.erase(this);

            if (parentSnapShot && !parentSnapShot->Children.contains(this))
                parentSnapShot->Children.insert(this);

            if (parentSnapShot == nullptr)
                ScreenObject->GetCanvas()->ParentlessScreenObjects.push_back(ScreenObject);

            Parent = parentSnapShot;

            // Convert screen space to local space with new parent
            if (parentSnapShot) {
                LocalPosition = ScreenToLocal(ScreenPosition);
                LocalRotation = ScreenRotation - parentSnapShot->Rotation;
                LocalScale.x = ScreenScale.x / parentSnapShot->Scale.x;
                LocalScale.y = ScreenScale.y / parentSnapShot->Scale.y;
            } else {
                LocalPosition = ScreenPosition;
                LocalRotation = ScreenRotation;
                LocalScale = ScreenScale;
            }
        }

        Internal::transformUI::~transformUI(){

            // Clear parent reference
            if (Parent && Parent->Children.contains(this)){
                Parent->Children.erase(this);
            }

            auto Copy = Children;
            Children.clear();
            for (transformUI* child : Copy){
                if (child && child->GetScreenObject()){
                    child->Parent = nullptr;
                    delete child->GetScreenObject();
                }
            }

            Copy.clear();
        }
    }
}