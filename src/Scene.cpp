#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

namespace obj{
    scene::~scene(){
        // Clear renderable components (they're owned by GameObjects)
        RenderableComponenets.clear();
        
        // Delete all game objects
        for (auto obj : GameObjects){
            if (obj) DestroyGameObject(obj);
        }

        GameObjects.clear();
        
        // Delete the camera
        if (ActiveCamera) DestroyCamera(ActiveCamera);
        
        // Clear windows (not owned by scene, just references)
        Windows.clear();
    }

    scene* CreateScene(){
        //create a new Scene
        scene* newSce = new scene();

        //add the window to the global list and increase the ID
        Internal::GlobalScenes.push_back(newSce);
        newSce->ID = Internal::Sce_ID;
        Internal::Sce_ID++;

        if (!newSce->ActiveCamera)
            newSce->ActiveCamera = CreateCamera();
        
        newSce->ActiveCamera->ActiveScene = newSce;

        return newSce;  // Returns a copy/move; no dangling reference
    }
    void DestroyScene(scene* Scene){
        if (Scene){
            delete Scene;
        }
    }
}