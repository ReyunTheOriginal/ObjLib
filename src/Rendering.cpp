#include "Rendering.hpp"

namespace obj{
    namespace Internal{
        struct renderCache{
            gameObject* obj = nullptr;
            renderLayer* Layer = nullptr;
        };
    } //Internal

    //Render all renderable objects
    void Render(){
        //clear all renderers
        for (auto& win : Internal::GlobalWindows){
           scene* Scene = win.second->GetScene();
           
           // Always clear to black for letterbox effect
           SDL_SetRenderDrawColor(win.second->SDLrenderer, 0, 0, 0, 255);
           SDL_RenderClear(win.second->SDLrenderer);
           
           if (Scene) {
                std::vector<Internal::renderCache> DrawOrder;
                DrawOrder.reserve(Scene->GameObjects.size());

                camera* ActiveCamera = Scene->ActiveCamera;
                if (ActiveCamera) {
                    //Draw the background first
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

                    for (auto& obj : Scene->GameObjects){
                        auto renderer = obj->GetComponent<spriteRenderer>();
                        if (renderer && renderer->Enabled) {
                            DrawOrder.push_back(Internal::renderCache{obj, &renderer->RenderLayer});
                        }
                    }

                    std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::renderCache& A, const Internal::renderCache& B){

                        if (A.Layer->Layer == B.Layer->Layer && A.Layer->Order == B.Layer->Order){
                            return A.obj->GetID() < B.obj->GetID();
                        }else if (A.Layer->Layer == B.Layer->Layer){
                            return A.Layer->Order < B.Layer->Order;
                        }else{
                            return A.Layer->Layer < B.Layer->Layer;
                        }
                    });

                    //loop through all components and Draw them
                    for (auto& obj : DrawOrder){
                        for (auto& com : obj.obj->Components){
                            if (com.second->Enabled)
                                com.second->Draw(win.second->SDLrenderer);
                        }
                    }
                }
           }
           
           //present all renderers
           SDL_RenderPresent(win.second->SDLrenderer);
        }
    }
} //obj