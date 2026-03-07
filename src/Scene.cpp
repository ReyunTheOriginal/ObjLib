#include "Scene.hpp"
#include "Gameobject.hpp"

namespace obj{
    scene* CreateScene(){
        //create a new Scene
        scene* newSce = new scene();

        //add the window to the global list and increase the ID
        Internal::SID++;
        Internal::GlobalScenes[Internal::SID] = newSce;

        return newSce;  // Returns a copy/move; no dangling reference
    }
    void DestroyScene(scene* Scene){
        if (Scene){
            delete Scene;
        }
    }
}