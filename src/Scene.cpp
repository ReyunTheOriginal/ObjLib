#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

namespace obj{
    scene::~scene(){
        for (auto obj : GameObjects){
            if (obj)DestroyGameObject(obj);
        }
    }

    scene* CreateScene(){
        //create a new Scene
        scene* newSce = new scene();

        //add the window to the global list and increase the ID
        Internal::GlobalScenes[Internal::SID] = newSce;
        newSce->ID = Internal::SID;
        Internal::SID++;

        if (!newSce->ActiveCamera)
            newSce->ActiveCamera = CreateCamera();

        return newSce;  // Returns a copy/move; no dangling reference
    }
    void DestroyScene(scene* Scene){
        if (Scene){
            delete Scene;
        }
    }
}