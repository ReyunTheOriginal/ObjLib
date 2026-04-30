#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include <algorithm>

namespace obj{
    scene::~scene(){
        std::erase(Internal::GlobalScenes, this);

        // Clear renderable components (they're owned by GameObjects)
        RenderableComponenets.clear();
        
        // Delete all game objects - use a snapshot to avoid issues
        auto GameObjectsSnapshot = GameObjects;
        for (auto obj : GameObjectsSnapshot){
            if (obj) delete obj; // Direct delete to avoid DestroyGameObject which modifies collections
        }
        GameObjects.clear();
        
        // Also remove from global list
        for (auto obj : GameObjectsSnapshot){
            std::erase(Internal::GlobalGameObjects, obj);
        }
        
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
        newSce->ID = Internal::Obj_ID;
        Internal::Obj_ID++;

        if (!newSce->ActiveCamera)
            newSce->ActiveCamera = CreateCamera();
        
        newSce->ActiveCamera->ActiveScene = newSce;

        return newSce;  // Returns a copy/move; no dangling reference
    }
}