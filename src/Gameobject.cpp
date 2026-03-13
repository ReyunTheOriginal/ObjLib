#include "Gameobject.hpp"
#include "Scene.hpp"

namespace obj{
    gameobject* CreateGameObject(scene* Scene){
        gameobject* newGam = new gameobject();
        newGam->Scene = Scene;
        if (Scene) {
            Scene->GameObjects.insert(newGam);
        }

        Internal::GID++;
        Internal::GlobalGameObjects[Internal::GID] = newGam;
        newGam->Name = "Gameobject #" + Internal::GID;
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
