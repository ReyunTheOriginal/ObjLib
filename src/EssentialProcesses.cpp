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
#include "Font.hpp"
#include "Sprite.hpp"
#include "UI/Canvas.hpp"
#include <vector>
#include <variant>

namespace obj{
    using AnyType = std::variant<
        Internal::component*,
        gameObject*,
        Font*,
        UI::canvas*,
        UI::screenObject*,
        sprite*
    >;

    std::vector<AnyType*> QueuedForDestruction;

    bool ObjMessages = true;

    bool IsQuiting = false;

    //Start up all necessary code to prepare the program
    int Init(){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        TTF_Init();
        MIX_Init();
        if (ObjMessages)std::cout << "\033[1;32m--- Initiated {\033[1;31mObjLib\033[1;32m} ---\033[0m" << "\n";
        return 0;
    }
    //update processes like physics, positions, and math
    void Update(){
        if (IsQuiting) return;

        for (int i=0;i<QueuedForDestruction.size();i++){
            delete QueuedForDestruction[i];
        }   
        QueuedForDestruction.clear();

        Input::Update();
        Time::Update();
        
        // Helper lambda to recursively update enabled game objects and their children
        std::function<void(Internal::transform*)> UpdateChildren = [&](Internal::transform* transform){
            if (!transform || !transform->GetGameObject() || !transform->GetGameObject()->Enabled) return;
            
            gameObject* obj = transform->GetGameObject();
            auto componentsCopy = obj->Components;
            
            for (auto& com : componentsCopy){
                if (com.second && com.second->Enabled){
                    if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                    com.second->Run();
                }
            }
            
            for (Internal::transform* child : transform->GetChildren()){
                UpdateChildren(child);
            }
        };
        
        // Update all game objects from their scene roots
        auto ScenesSnapshot = Internal::GlobalScenes;
        for (auto& scene : ScenesSnapshot){
            if (scene && scene->ObjectParent){
                for (Internal::transform* obj : scene->ObjectParent->GetChildren()){
                    UpdateChildren(obj);
                }
            }
        }

        // Create a snapshot to avoid iterator invalidation if objects are destroyed during update
        auto ScreenObjectsSnapshot = Internal::GlobalScreenObjects;
        for (auto& obj : ScreenObjectsSnapshot){
            if (obj && obj->Enabled){
                //loop through all components and run them
                for (auto& com : obj->Components){
                    if (com.second && com.second->Enabled){
                        if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                        com.second->Run();
                    }
                }
            }
        }

        //set all children/parent relationships to maintain correctness
        // Create a snapshot of windows too
        auto WindowsSnapshot = Internal::GlobalWindows;
        for (auto& win : WindowsSnapshot){
            if (win){
                camera* ActiveCamera = win->ActiveCamera;
                if (ActiveCamera) {
                    ActiveCamera->ActiveWindow = win;
                }
            }
        }
    }
    
    //safely quit the entire program
    void Quit(){
        QueuedForDestruction.clear();

        //clear all windows
        auto windows = Internal::GlobalWindows;  // Make a copy
        for (auto& win : windows){
            delete win;
        }
        Internal::GlobalWindows.clear();
        
        //clear all scenes (this will delete all gameobjects through scene destructors)
        auto scenes = Internal::GlobalScenes;  // Make a copy

        for (auto& scene : scenes)
            delete scene;

        Internal::GlobalScenes.clear();
        Internal::GlobalGameObjects.clear(); // These are already deleted by scene destructors

        SDL_Quit();
        if (ObjMessages)std::cout << "\033[1;32m--- Exited {\033[1;31mObjLib\033[1;32m} ---\033[0m" << "\n";
        std::exit(0);
    }
}