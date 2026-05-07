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

    //Start up all necessary code to prepare the program
    int Init(){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        TTF_Init();
        MIX_Init();
        if (ObjMessages)std::cout << "\033[1;32m--- Initiated {\033[1;31mObjLib\033[1;32m} ---\033[0m" << "\n";
        return 0;
    }

    void UpdateTransform(Internal::transform* trans){
        if (!trans) return;
            
        // Update world position, rotation, and scale based on parent
        trans->SetWorldPositionRaw(trans->LocalToWorld(trans->LocalPosition));
        
        trans->SetWorldRotationRaw(trans->GetParent() ? 
            trans->GetParent()->GetWorldRotation() + trans->LocalRotation : 
            trans->LocalRotation);
        
        trans->SetWorldScaleRaw(trans->GetParent() ? 
            trans->GetParent()->GetWorldScale() * trans->LocalScale : 
            trans->LocalScale);
        
        // Recursively update children
        for (auto child : trans->GetChildren()){
            UpdateTransform(child);
        }
    }

    void UpdateUITransforms(UI::Internal::transformUI* trans){
        if (!trans) return;
            
        // Update world position, rotation, and scale based on parent
        vector2 worldPos = trans->LocalPosition;
        if (trans->GetParent()){
            // Rotate by parent's rotation
            float rad = Math::Deg2Rad(trans->GetParent()->GetWorldRotation());
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);
            worldPos = { worldPos.x * cs - worldPos.y * sn, worldPos.x * sn + worldPos.y * cs };
            
            // Scale by parent's scale
            worldPos.x *= trans->GetParent()->GetWorldScale().x;
            worldPos.y *= trans->GetParent()->GetWorldScale().y;
            
            // Add parent's position
            worldPos += trans->GetParent()->GetWorldPosition();
        }
        trans->SetWorldPositionRaw(worldPos);
        
        trans->SetWorldRotationRaw(trans->GetParent() ? 
            trans->GetParent()->GetWorldRotation() + trans->LocalRotation : 
            trans->LocalRotation);
        
        trans->SetWorldScaleRaw(trans->GetParent() ? 
            trans->GetParent()->GetWorldScale() * trans->LocalScale : 
            trans->LocalScale);
        
        // Recursively update children
        for (auto child : trans->GetChildren()){
            UpdateUITransforms(child);
        }
    }

    void RunComponents(Internal::transform* transform){
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
            RunComponents(child);
        }
    }

    void RunUIComponents(UI::Internal::transformUI* transform){
        if (!transform || !transform->GetScreenObject() || !transform->GetScreenObject()->Enabled) return;
            
        UI::screenObject* obj = transform->GetScreenObject();
        auto componentsCopy = obj->Components;
        
        for (auto& com : componentsCopy){
            if (com.second && com.second->Enabled){
                if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                com.second->Run();
            }
        }
        
        for (UI::Internal::transformUI* child : transform->GetChildren()){
            RunUIComponents(child);
        }
    }

    //Apply things like Phyiscs, Hiararchies, and Delete things
    void Apply(){
        for (int i=0;i<QueuedForDestruction.size();i++){
            std::visit([](auto* ptr) {
                delete ptr;
            }, *QueuedForDestruction[i]);
            delete QueuedForDestruction[i];
        }   
        QueuedForDestruction.clear();
        
        // First pass: Update ALL transforms recursively (parents before children)
        auto ScenesSnapshot = Internal::GlobalScenes;
        for (auto& scene : ScenesSnapshot){
            if (scene){
                // Update all objects, handling both parentless and parented
                for (gameObject* obj : scene->GameObjects){
                    // Only update if object has no parent (root objects)
                    if (obj->Transform && !obj->Transform->GetParent()){
                        UpdateTransform(obj->Transform);
                    }
                }
            }
        }

        // Second pass: Update components (after all transforms are correct)
        for (auto& scene : ScenesSnapshot){
            if (scene){
                for (gameObject* obj : scene->GameObjects){
                    if (obj->Transform && !obj->Transform->GetParent()){
                        RunComponents(obj->Transform);
                    }
                }
            }
        }

        // Update UI from parentless screen objects
        for (auto& Win : Internal::GlobalWindows){
            if (Win->ActiveCamera->ActiveCanvas){
                // First pass: Update UI transforms recursively
                for (UI::screenObject* obj : Win->ActiveCamera->ActiveCanvas->ParentlessScreenObjects){
                    if (obj->UITransform && !obj->UITransform->GetParent()){
                        UpdateUITransforms(obj->UITransform);
                    }
                }
                
                // Second pass: Update UI components
                for (UI::screenObject* obj : Win->ActiveCamera->ActiveCanvas->ParentlessScreenObjects){
                    if (obj->UITransform && !obj->UITransform->GetParent()){
                        RunUIComponents(obj->UITransform);
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
    
    //update processes like Input, Time, and FPS
    void Update(){
        Input::Update();
        Time::Update();
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
        if (ObjMessages)std::cout << "\n" << "\033[1;32m--- Exited {\033[1;31mObjLib\033[1;32m} ---\033[0m" << "\n";
        std::exit(0);
    }
}