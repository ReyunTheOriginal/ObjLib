#include "GameObject.hpp"
#include <algorithm>

#include "Scene.hpp"
#include "Math.hpp"
#include "GlobalLists.hpp"
#include "Components/ComponentBase.hpp"

namespace obj{
    gameObject::~gameObject(){
        // Remove from scene
            if (Scene){
                std::erase(Scene->GameObjects, this);
            }
            
            // Remove from global list
            std::erase(Internal::GlobalGameObjects, this);

            for(auto& com : Components){
                if (com.second){
                    delete com.second;
                }
            }
            Components.clear();
    }
    
    gameObject* CreateGameObject(scene* Scene){
        if (!Scene)return nullptr;

        gameObject* newGam = new gameObject();
        newGam->Scene = Scene;
        Scene->GameObjects.push_back(newGam);

        newGam->Transform = new Internal::transform;

        Internal::GlobalGameObjects.push_back(newGam);
        newGam->ID = Internal::Obj_ID;
        newGam->Name = "GameObject #" + Internal::Obj_ID;
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
