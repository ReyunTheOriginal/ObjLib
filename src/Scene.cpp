#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include <algorithm>

namespace obj{
    scene::~scene(){
        std::erase(Internal::GlobalScenes, this);
        
        // Delete all game objects - use a snapshot to avoid issues
        auto GameObjectsSnapshot = ParentlessGameObjects;
        
        for (gameObject* obj : GameObjectsSnapshot){
            obj->SetScene(nullptr);
            delete obj;
        }

        GameObjects.clear();
        GameObjectsSnapshot.clear();
        Windows.clear();
    }

    scene* CreateScene(){
        //create a new Scene
        scene* newSce = new scene();

        //add the window to the global list and increase the ID
        Internal::GlobalScenes.push_back(newSce);
        newSce->ID = Internal::Obj_ID;
        Internal::Obj_ID++;

        newSce->Name = "Scene" + std::to_string(newSce->ID);

        return newSce;  // Returns a copy/move; no dangling reference
    }
}