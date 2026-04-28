#include "GameObject.hpp"
#include "Scene.hpp"

namespace obj{
    gameObject* CreateGameObject(scene* Scene){
        gameObject* newGam = new gameObject();
        newGam->Scene = Scene;
        if (Scene) {
            Scene->GameObjects.insert(newGam);
        }

        Internal::GlobalGameObjects[Internal::GID] = newGam;
        newGam->ID = Internal::GID;

        Internal::GID++;
        
        newGam->Name = "GameObject #" + Internal::GID;
        return newGam;
    }

    void DestroyGameObject(gameObject* GameObject){
        if (GameObject){
            for(auto com : GameObject->Components){
                com.second->Destroy();
            }
            delete GameObject;
        }
    }
}
