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
            if (Scene) {
                camera* ActiveCamera = Scene->GetActiveCamera();
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
    //Render all renderable objects
    void Render(){
        //clear all renderers
        for (auto& win : Internal::GlobalWindows){
           scene* Scene = win.second->GetScene();
           
           // If no scene, just render black
           SDL_SetRenderDrawColor(win.second->SDLrenderer,0,0,0,255);
           SDL_RenderClear(win.second->SDLrenderer);
           
           if (Scene) {
               camera* ActiveCamera = Scene->GetActiveCamera();
               if (ActiveCamera) {
                   color bgColor = Scene->BackGroundColor;
                   ActiveCamera->ActiveWindow = win.second;

                   SDL_SetRenderDrawColor(win.second->SDLrenderer,
                       (Uint8)bgColor.r,
                       (Uint8)bgColor.g,
                       (Uint8)bgColor.b,
                       255
                   );

                   vector2 Logical = ActiveCamera->GetResolution();

                   SDL_FRect rect = {0, 0, Logical.x, Logical.y};
                   SDL_RenderFillRect(win.second->SDLrenderer, &rect);
               }
           }
        }

        for (auto& obj : Internal::GlobalGameObjects){
            //loop through all components and Draw them
            for (auto& com : obj.second->Components){
                if (com.second->Enabled)com.second->Draw();
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