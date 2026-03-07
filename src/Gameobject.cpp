#include "Gameobject.hpp"
#include "Scene.hpp"

namespace obj{
    gameobject* CreateGameObject(scene* Scene){
        gameobject* newGam = new gameobject();
        newGam->Scene = Scene;
        Scene->GID++;
        Scene->GameObjects[Scene->GID] = newGam;

        Internal::GID++;
        Internal::GlobalGameObjects[Internal::GID] = newGam;
        return newGam;
    }

    void DestroyGameObject(gameobject* GameObject){
        if (GameObject){
            for(auto com : GameObject->Components){
                com.second->Destroy();
            }
            delete GameObject;
        }
    }
}
