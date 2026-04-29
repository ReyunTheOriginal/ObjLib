#include "EssentialProcesses.hpp"

namespace obj{
    //Start up all necessary code to prepare the program
    int Init(){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        std::cout << "\033[1;32m--- Initiated \033[1;31mObjLib\033[1;32m ---\033[0m" << "\n";
        return 0;
    }
    //update processes like physics, positions, and math
    void Update(){
        Input::Update();
        Time::Update();
        
        for (auto& obj : Internal::GlobalGameObjects){
            if (obj){
                //loop through all components and run them
                for (auto& com : obj->Components){
                    if (com.second){
                        if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                        if (com.second->Enabled)com.second->Run();
                    }
                }
            }
        }

        //set all children/parent relationships to maintain correctness
        for (auto& win : Internal::GlobalWindows){
            scene* Scene = win->GetScene();
            if (Scene) {
                camera* ActiveCamera = Scene->ActiveCamera;
                if (ActiveCamera) {
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
        //clear all scenes (this will delete all gameobjects through scene destructors)
        for (auto& scene : Internal::GlobalScenes){
            DestroyScene(scene);
        }
        Internal::GlobalScenes.clear();
        Internal::GlobalGameObjects.clear(); // These are already deleted by scene destructors

        //clear all windows
        for (auto& win : Internal::GlobalWindows){
            DestroyWindow(win);
        }
        Internal::GlobalWindows.clear();

        SDL_Quit();

        std::cout << "\033[1;32m--- Exited \033[1;31mObjLib\033[1;32m ---\033[0m" << "\n";
        std::exit(0);
    }
}