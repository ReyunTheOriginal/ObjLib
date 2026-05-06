#include "GameObject.hpp"
#include <algorithm>

#include "Scene.hpp"
#include "Math.hpp"
#include "GlobalLists.hpp"
#include "Components/ComponentBase.hpp"

namespace obj{
    void Internal::transform::SetParent(transform* ParentToSet){
        if (Parent){
            if (Parent->Children.contains(this)){
                Parent->Children.erase(this);
            }
        }

        if (ParentToSet == nullptr)
            ParentToSet = GameObject->GetScene()->ObjectParent;

        if (!ParentToSet->Children.contains(this)){
            ParentToSet->Children.insert(this);
        }

        Parent = ParentToSet;
    }
    
    gameObject::~gameObject(){
        // Remove from scene
            if (Scene){
                std::erase(Scene->GameObjects, this);
            }
            
            // Remove from global list
            std::erase(Internal::GlobalGameObjects, this);

            // Delete all components
            for(auto& com : Components){
                if (com.second){
                    delete com.second;
                }
            }

            // Delete Transform last (it will handle cascading deletion of children)
            if (Transform){
                delete Transform;
            }
            
            Components.clear();
    }
    
    gameObject* CreateGameObject(scene* Scene, Internal::transform* Parent){
        if (!Scene)return nullptr;

        gameObject* newGam = new gameObject();
        newGam->Scene = Scene;
        Scene->GameObjects.push_back(newGam);

        newGam->Transform = new Internal::transform(newGam);

        if (Parent == nullptr){
            newGam->Transform->SetParent(Scene->ObjectParent);
        }else{
            newGam->Transform->SetParent(Parent);
        }

        Internal::GlobalGameObjects.push_back(newGam);
        newGam->ID = Internal::Obj_ID;
        newGam->Name = "GameObject #" + std::to_string(Internal::Obj_ID);
        Internal::Obj_ID++;
        return newGam;
    }

    void gameObject::SetScene(scene* SceneToSendTo){
        if (Scene){
            std::erase(Scene->GameObjects, this);

            SceneToSendTo->GameObjects.push_back(this);
        }
    }
}
