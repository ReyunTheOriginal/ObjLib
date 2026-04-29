#include "GameObject.hpp"
#include "Scene.hpp"

namespace obj{
    gameObject* CreateGameObject(scene* Scene){
        gameObject* newGam = new gameObject();
        newGam->Scene = Scene;
        if (Scene) {
            Scene->GameObjects.push_back(newGam);
        }

        Internal::GlobalGameObjects.push_back(newGam);
        newGam->ID = Internal::Gam_ID;

        Internal::Gam_ID++;

        newGam->Name = "GameObject #" + Internal::Gam_ID;
        return newGam;
    }

    void gameObject::SendToScene(scene* SceneToSendTo){
        if (Scene){
            std::erase(Scene->GameObjects, this);

            SceneToSendTo->GameObjects.push_back(this);
        }
    }

    void DestroyGameObject(gameObject* GameObject){
        if (GameObject){
            delete GameObject; // Destructor will handle cleanup
        }
    }
}
