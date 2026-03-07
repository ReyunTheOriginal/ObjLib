#include "EssentialProcesses.hpp"

namespace obj{
    //Start up all necessary code to prepare the program
    int Init(){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        std::cout << "---Initiated OBJLib---\n";
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
            Scene->Window = win.second;
            for (auto& obj : Scene->GameObjects){
                obj.second->Scene = Scene;
                for (auto& com : obj.second->Components){
                    com.second->GameObject = obj.second;
                }
            }
        }
    }
    //Render all renderable objects
    void Render(){
        //clear all renderers
        for (auto& win : Internal::GlobalWindows){
            color bgColor = win.second->GetScene()->BackGroundColor;
            SDL_SetRenderDrawColor(win.second->SDLrenderer, (Uint8)(bgColor.r * 255), (Uint8)(bgColor.g * 255), (Uint8)(bgColor.b * 255), (Uint8)(255));
            SDL_RenderClear(win.second->SDLrenderer);
        }

        for (auto& obj : Internal::GlobalGameObjects){
            //loop through all components and Draw them
            for (auto& com : obj.second->Components){
                if (com.second->Enabled)com.second->Draw();
            }
            //loop through all components and Draw their debug states if in a debug window
            if (obj.second != nullptr && 
                obj.second->Scene != nullptr && 
                obj.second->Scene->Window != nullptr &&
                obj.second->Scene->Window->Debug) {
                for (auto& com : obj.second->Components){
                    if (com.second->Enabled)com.second->DebugDraw();
                }
            }
        }

        //present all renderers
        for (auto& win : Internal::GlobalWindows){
            SDL_RenderPresent(win.second->SDLrenderer);
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