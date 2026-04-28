#include "EssentialProcesses.hpp"

namespace obj{
    //Start up all necessary code to prepare the program
    int Init(){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        std::cout << "--- Initiated OBJLib ---\n";
        return 0;
    }
    //update processes like physics, positions, and math
    void Update(){
        Input.Update();
        
        for (auto& obj : Internal::GlobalGameObjects){
            if (obj.second){
                //loop through all components and run them
                for (auto& com : obj.second->Components){
                    if (com.second){
                        if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                        if (com.second->Enabled)com.second->Run();
                    }
                }
            }
        }

        //set all children/parent relationships to maintain correctness
        for (auto& win : Internal::GlobalWindows){
            scene* Scene = win.second->GetScene();
            if (Scene) {
                camera* ActiveCamera = Scene->ActiveCamera;
                if (ActiveCamera) {
                    if (Scene->Windows.find(win.second) != Scene->Windows.end()){
                        Scene->Windows.insert(win.second);
                    }
                    ActiveCamera->ActiveScene = Scene;
                }
                for (auto& obj : Scene->GameObjects){
                    obj->Scene = Scene;
                    for (auto& com : obj->Components){
                        com.second->GameObject = obj;
                    }
                }
            }
        }
    }
    
    //safely quit the entire program
    void Quit(){
        //clear all gameobjects
        for (auto& obj : Internal::GlobalGameObjects){
            DestroyGameObject(obj.second);
        }
        Internal::GlobalGameObjects.clear();

        //clear all scenes
        for (auto& obj : Internal::GlobalScenes){
            DestroyScene(obj.second);
        }
        Internal::GlobalScenes.clear();

        //clear all windows
        for (auto& obj : Internal::GlobalWindows){
            DestroyWindow(obj.second);
        }
        Internal::GlobalWindows.clear();

        SDL_Quit();
        std::exit(0);
    }
}