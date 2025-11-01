#include "Objects.hpp"

namespace obl{
    unsigned int gameObject::NextID = 0;

    namespace Internal{

        gameObject* transform::SetParent(gameObject* parent){
            Parent = parent;
            if (parent){
                Parent->Transform->Children.erase(GameObject->GetID());
                parent->Transform->SetChild(parent);
            }
            return parent;
        }
        gameObject* transform::SetChild(gameObject* child){
            Children[child->GetID()] = child;
            return child;
        }

        gameObject* transform::GetParent(){return Parent;}
        gameObject* transform::GetChild(int childID){
            if (Children.contains(childID))
                return Children.at(childID);
            else
                return nullptr;
        }

        vector2 transform::WorldToLocal(vector2 WorldPos){
            if (!Parent) return WorldPos;
            vector2 Local;

            Local = Parent? WorldPos - Parent->Transform->Position : WorldPos;

            float rad = Math::Deg2Rad(Parent->Transform->Rotation);
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);

            Local /= Size;

            Local =  { Local.x * cs - Local.y * sn, Local.x * sn + Local.y * cs };
            return Local;
        }
        vector2 transform::LocalToWorld(vector2 LocalPos){
            if (!Parent) return LocalPos;
            vector2 World;

            World = LocalPos;

            float rad = Math::Deg2Rad(-Parent->Transform->Rotation);
            float cs = Math::Cos(-rad);
            float sn = Math::Sin(-rad);

            World *= Size;

            World =  { World.x * cs - World.y * sn, World.x * sn + World.y * cs };
            return World + Parent->Transform->Position;
        }
    }

}