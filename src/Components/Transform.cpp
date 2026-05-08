#include "Components/Transform.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"

namespace obj{
    Internal::transform::~transform(){
        // Delete all children's GameObjects first
        auto Copy = Children;
        Children.clear();
        for (auto child : Copy){
            if (child && child->GetGameObject()){
                child->Parent = nullptr;
                delete child->GetGameObject();
            }
        }
        Copy.clear();
        
        // Clear parent reference
        if (Parent && Parent->Children.contains(this)){
            Parent->Children.erase(this);
        }
    }

    namespace Internal{
        vector2 transform::WorldToLocal(vector2 WorldPos){
            if (!Parent) return WorldPos;
            vector2 Local;

            Local = WorldPos - Parent->Position;

            float rad = Math::Deg2Rad(-Parent->Rotation);
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);

            Local =  { Local.x * cs - Local.y * sn, Local.x * sn + Local.y * cs };
            Local.x /= Parent->Scale.x;
            Local.y /= Parent->Scale.y;
            return Local;
        }
        vector2 transform::LocalToWorld(vector2 LocalPos){
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

        void transform::SetWorldPosition(vector2 Pos){
            LocalPosition = WorldToLocal(Pos);
            // Immediately update world position
            Position = LocalToWorld(LocalPosition);
        }
        void transform::SetWorldScale(vector2 Scale){
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
        void transform::SetWorldRotation(float Rotation){
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
    
        void transform::SetParent(transform* ParentToSet){
            transform* parentSnapShot = ParentToSet;

            std::erase(GameObject->GetScene()->ParentlessGameObjects, GameObject);

            if ((parentSnapShot && parentSnapShot->IsDescendantOf(this)) || parentSnapShot == this)
                parentSnapShot = nullptr;

            if (Parent && Parent->Children.contains(this))
                Parent->Children.erase(this);

            if (parentSnapShot && !parentSnapShot->Children.contains(this))
                parentSnapShot->Children.insert(this);

            if (parentSnapShot == nullptr)
                GameObject->GetScene()->ParentlessGameObjects.push_back(GameObject);

            Parent = parentSnapShot;

            LocalPosition = WorldToLocal(LocalToWorld(LocalPosition));
            LocalRotation = parentSnapShot ? Rotation - parentSnapShot->Rotation : Rotation;
            LocalScale.x = parentSnapShot ? Scale.x / parentSnapShot->Scale.x : Scale.x;
            LocalScale.y = parentSnapShot ? Scale.y / parentSnapShot->Scale.y : Scale.y;
        }
    }
}