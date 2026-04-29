#include "Rendering.hpp"
#include <iostream>

namespace obj{
    namespace Internal{
        struct renderCache{
            component* com = nullptr;
            renderLayer* Layer = nullptr;
        };
    } //Internal

    //Render all renderable objects
    void Render(){
        //clear all renderers
        for (auto& win : Internal::GlobalWindows){
           scene* Scene = win->GetScene();
           
           // Always clear to black for letterbox effect
           SDL_SetRenderDrawColor(win->SDLrenderer, 0, 0, 0, 255);
           SDL_RenderClear(win->SDLrenderer);
           
           if (Scene){
                std::vector<Internal::renderCache> DrawOrder;
                DrawOrder.reserve(Scene->RenderableComponenets.size());

                camera* ActiveCamera = Scene->ActiveCamera;
                if (ActiveCamera) {
                    vector2 Logical = ActiveCamera->GetResolution();

                    //Draw the background first<
                    color bgColor = Scene->BackGroundColor;

                    SDL_SetRenderDrawColor(win->SDLrenderer,
                        (Uint8)bgColor.r,
                        (Uint8)bgColor.g,
                        (Uint8)bgColor.b,
                        255
                    );

                    SDL_FRect rect = {0, 0, Logical.x, Logical.y};
                    SDL_RenderFillRect(win->SDLrenderer, &rect);

                    for (auto& renderer : Scene->RenderableComponenets){
                        if (renderer) {
                            DrawOrder.push_back(Internal::renderCache{renderer, renderer->RenderLayer});
                        }
                    }

                    std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::renderCache& A, const Internal::renderCache& B){

                        if (A.Layer->Layer == B.Layer->Layer && A.Layer->Order == B.Layer->Order){
                            return A.com->GameObject->GetID() < B.com->GameObject->GetID();
                        }else if (A.Layer->Layer == B.Layer->Layer){
                            return A.Layer->Order < B.Layer->Order;
                        }else{
                            return A.Layer->Layer < B.Layer->Layer;
                        }
                    });

                    //loop through all components and Draw them
                    for (auto& renderer : DrawOrder){
                        renderer.com->Draw(win->SDLrenderer);
                    }

                    if (win->Debug){
                        //loop through all components and Draw them
                        for (auto& renderer : DrawOrder){
                            renderer.com->DebugDraw(win->SDLrenderer);
                        }
                    }
                }
           }
           
           //present all renderers
           SDL_RenderPresent(win->SDLrenderer);
        }
        
        // Clear renderable components after all windows are rendered
        for (auto& scene : Internal::GlobalScenes){
            scene->RenderableComponenets.clear();
        }
    }
} //obj