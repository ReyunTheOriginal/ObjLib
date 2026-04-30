#include "Rendering.hpp"
#include <iostream>
#include <algorithm>

#include "Component.hpp"
#include "GlobalTypes.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "UI/UIBase.hpp"

namespace obj{

    namespace Internal{
        struct renderCache{
            component* com = nullptr;
            renderSorter* Layer = nullptr;
        };

        struct screenRenderCache{
            UI::Internal::screenComponent* com = nullptr;
            renderSorter* Layer = nullptr;
        };
    } //Internal

    void RenderCanvases(window* Win){
        if (!Win)return;

        for (window* win : Internal::GlobalWindows){

            scene* Scene = win->GetScene();
            if (!Scene) return;

            camera* ActiveCamera = Scene->ActiveCamera;
            if (!ActiveCamera)return;

            UI::canvas* Canvas = ActiveCamera->ActiveCanvas;
            if (!Canvas)return;

            std::vector<Internal::screenRenderCache> DrawOrder;
            DrawOrder.reserve(Canvas->RenderableUI.size());

            for (UI::Internal::screenComponent* com : Canvas->RenderableUI){
                if (com)DrawOrder.push_back(Internal::screenRenderCache{com, com->RenderLayer});
            }

            std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::screenRenderCache& A, const Internal::screenRenderCache& B){
                if (A.Layer->Layer == B.Layer->Layer && A.Layer->Order == B.Layer->Order){
                    return A.com->ScreenObject->GetID() < B.com->ScreenObject->GetID();
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

    void RenderDefaultWorld(window* Win){
        if (!Win)return;

        scene* Scene = Win->GetScene();
        
        // Always clear to black for letterbox effect
        SDL_SetRenderDrawColor(Win->SDLrenderer, 0, 0, 0, 255);
        SDL_RenderClear(Win->SDLrenderer);

        camera* ActiveCamera = Scene->ActiveCamera;
        
        if (Scene && ActiveCamera){
            vector2 Logical = ActiveCamera->GetResolution();

            //Draw the background first<
            color bgColor = Scene->BackGroundColor;

            SDL_SetRenderDrawColor(Win->SDLrenderer,
                (Uint8)bgColor.r,
                (Uint8)bgColor.g,
                (Uint8)bgColor.b,
                255
            );

            SDL_FRect rect = {0, 0, Logical.x, Logical.y};
            SDL_RenderFillRect(Win->SDLrenderer, &rect);
        }

    }

    void RenderWorldObjects(window* Win){
        if (!Win)return;

        scene* Scene = Win->GetScene();

        camera* ActiveCamera = Scene->ActiveCamera;
        
        if (Scene && ActiveCamera){
            std::vector<Internal::renderCache> DrawOrder;
            DrawOrder.reserve(Scene->RenderableComponenets.size());

            for (auto& com : Scene->RenderableComponenets){
                if (com) {
                    DrawOrder.push_back(Internal::renderCache{com, com->RenderLayer});
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
                renderer.com->Draw(Win->SDLrenderer);
            }

            if (Win->Debug){
                //loop through all components and Draw them
                for (auto& renderer : DrawOrder){
                    renderer.com->DebugDraw(Win->SDLrenderer);
                }
            }
        }

    }// RenderWorldObjects()

    void ClearRenderables(){
        // Clear renderable components after all windows are rendered
        auto ScenesSnapshot = Internal::GlobalScenes;
        for (auto& scene : ScenesSnapshot){
            if (scene){
                scene->RenderableComponenets.clear();

                camera* cam = scene->ActiveCamera;

                if (cam){
                    UI::canvas* canvas = cam->ActiveCanvas;

                    if (canvas){
                        canvas->RenderableUI.clear();
                    }
                }
            }
        }
    }

    //Render all renderable objects
    void Render(){
        const auto& windows = Internal::GlobalWindows;

        for (window* Win : windows){
            RenderDefaultWorld(Win);
            RenderWorldObjects(Win);
            RenderCanvases(Win);

            //present all renderers
            SDL_RenderPresent(Win->SDLrenderer);
        }
        ClearRenderables();
    }


} //obj