#include "Components/Transform.hpp"
#include "GameObject.hpp"

namespace obj{
    Internal::transform::~transform(){
        // Delete all children's GameObjects first
        std::vector<Internal::transform*> childrenCopy(Children.begin(), Children.end());
        for (auto child : childrenCopy){
            if (child && child->GetGameObject()){
                delete child->GetGameObject();
            }
        }
        Children.clear();
        
        // Clear parent reference
        if (Parent && Parent->Children.contains(this)){
            Parent->Children.erase(this);
        }
    }
}