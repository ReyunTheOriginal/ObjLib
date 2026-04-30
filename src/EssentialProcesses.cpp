#include "EssentialProcesses.hpp"
#include <algorithm>

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "GlobalLists.hpp"
#include "Scene.hpp"
#include "GlobalTypes.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "Time.hpp"

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
        
        // Create a snapshot to avoid iterator invalidation if objects are destroyed during update
        auto GameObjectsSnapshot = Internal::GlobalGameObjects;
        for (auto& obj : GameObjectsSnapshot){
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

        // Create a snapshot to avoid iterator invalidation if objects are destroyed during update
        auto ScreenObjectsSnapshot = Internal::GlobalScreenObjects;
        for (auto& obj : ScreenObjectsSnapshot){
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
        // Create a snapshot of windows too
        auto WindowsSnapshot = Internal::GlobalWindows;
        for (auto& win : WindowsSnapshot){
            if (win){
                scene* Scene = win->GetScene();
                if (Scene) {
                    camera* ActiveCamera = Scene->ActiveCamera;
                    if (ActiveCamera) {
                        ActiveCamera->ActiveScene = Scene;
                    }
                }
            }
        }
    }
    
    //safely quit the entire program
    void Quit(){
        //clear all scenes (this will delete all gameobjects through scene destructors)
        for (auto& scene : Internal::GlobalScenes){
            delete scene;
        }
        Internal::GlobalScenes.clear();
        Internal::GlobalGameObjects.clear(); // These are already deleted by scene destructors

        //clear all windows
        for (auto& win : Internal::GlobalWindows){
            delete win;
        }
        Internal::GlobalWindows.clear();

        SDL_Quit();

        std::cout << "\033[1;32m--- Exited \033[1;31mObjLib\033[1;32m ---\033[0m" << "\n";
        std::exit(0);
    }
}