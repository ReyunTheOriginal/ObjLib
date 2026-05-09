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
#include "Camera/Camera.hpp"
#include "Camera/CameraComponent.hpp"
#include "UI/UIBase.hpp"
#include "Time.hpp"
#include "Font.hpp"
#include "Sprite.hpp"
#include <vector>
#include <variant>

namespace obj{
    using AnyType = std::variant<
        component*,
        gameObject*,
        font*,
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
        vector2 ScreenPos = trans->LocalPosition;
        if (trans->GetParent()){
            // Rotate by parent's rotation
            float rad = Math::Deg2Rad(-trans->GetParent()->GetScreenRotation());  // Negate for clockwise
            float cs = Math::Cos(rad);
            float sn = Math::Sin(rad);
            ScreenPos = { ScreenPos.x * cs - ScreenPos.y * sn, ScreenPos.x * sn + ScreenPos.y * cs };
            
            // Scale by parent's scale
            ScreenPos.x *= trans->GetParent()->GetScreenScale().x;
            ScreenPos.y *= trans->GetParent()->GetScreenScale().y;
            
            // Add parent's position
            ScreenPos += trans->GetParent()->GetScreenPosition();
        }
        trans->SetScreenPositionRaw(ScreenPos);
        
        trans->SetScreenRotationRaw(trans->GetParent() ? 
            trans->GetParent()->GetScreenRotation() + trans->LocalRotation : 
            trans->LocalRotation);
        
        trans->SetScreenScaleRaw(trans->GetParent() ? 
            trans->GetParent()->GetScreenScale() * trans->LocalScale : 
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

        // Update UI from parentless screen objects
        for (auto& Win : Internal::GlobalWindows){
            if (!Win)return;

            scene* scene = Win->GetScene();
            camera* camera = Win->GetCamera();

            if (camera){
                auto componentsCopy = camera->Components;
                for (auto& com : componentsCopy){
                    if (com.second && com.second->Enabled){
                        if (!com.second->DidInit){com.second->Init(); com.second->DidInit = true;}
                        com.second->Run();
                    }
                }

                if (camera->GetCanvas()){
                    // First pass: Update UI transforms recursively
                    for (UI::screenObject* obj : camera->GetCanvas()->GetParentlessScreenObjects()){
                        if (obj->UITransform && !obj->UITransform->GetParent()){
                            UpdateUITransforms(obj->UITransform);
                        }

                        if (obj->UITransform && !obj->UITransform->GetParent()){
                            RunUIComponents(obj->UITransform);
                        }
                    }

                    // Final transform update pass - objects created during component execution need their transforms calculated
                    for (UI::screenObject* obj : camera->GetCanvas()->GetParentlessScreenObjects()){
                        if (obj->UITransform && !obj->UITransform->GetParent()){
                            UpdateUITransforms(obj->UITransform);
                        }
                    }

                }
            }

            if (scene){
                // Update all objects, handling both parentless and parented
                for (gameObject* obj : scene->GetParentlessGameObjects()){
                    // Only update if object has no parent (root objects)
                    if (obj->Transform && !obj->Transform->GetParent()){
                        UpdateTransform(obj->Transform);
                    }

                    if (obj->Transform && !obj->Transform->GetParent()){
                        RunComponents(obj->Transform);
                    }
                }
                
                // Final transform update pass - objects created during component execution need their transforms calculated
                for (gameObject* obj : scene->GetParentlessGameObjects()){
                    if (obj->Transform && !obj->Transform->GetParent()){
                        UpdateTransform(obj->Transform);
                    }
                }
            }

            if (Win->Debug)
                Win->DebugDisplay();
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

        SDL_Quit();
        if (ObjMessages)std::cout << "\n" << "\033[1;32m--- Exited {\033[1;31mObjLib\033[1;32m} ---\033[0m" << "\n";
        std::exit(0);
    }
}