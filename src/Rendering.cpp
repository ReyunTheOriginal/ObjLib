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

        scene* Scene = Win->GetScene();
        if (!Scene) return;

        camera* ActiveCamera = Win->ActiveCamera;
        if (!ActiveCamera)return;

        UI::canvas* Canvas = ActiveCamera->ActiveCanvas;
        if (!Canvas)return;

        std::vector<Internal::screenRenderCache> DrawOrder;
        DrawOrder.reserve(Canvas->UI.size());

        for (UI::screenObject* obj : Canvas->UI){
            for (auto& com : obj->Components){
                if (com.second)DrawOrder.push_back(Internal::screenRenderCache{com.second, com.second->RenderLayer});
            }
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
        auto copy = DrawOrder;
        for (auto& renderer : copy){
            renderer.com->Draw(Win);
        }

        if (Win->Debug){
            //loop through all components and Draw them
            auto copy = DrawOrder;
            for (auto& renderer : copy){
                renderer.com->DebugDraw(Win);
            }
        }

    }

    void RenderDefaultWorld(window* Win){
        if (!Win)return;
        
        // Always clear to black for letterbox effect
        SDL_SetRenderDrawColor(Win->SDLrenderer, 0, 0, 0, 255);
        SDL_RenderClear(Win->SDLrenderer);

        scene* Scene = Win->GetScene();
        if (!Scene) return;

        camera* ActiveCamera = Win->ActiveCamera;
        
        if (ActiveCamera){
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

        if (!Scene) return;

        camera* ActiveCamera = Win->ActiveCamera;
        
        if (ActiveCamera){
            std::vector<Internal::renderCache> DrawOrder;
            DrawOrder.reserve(Scene->GameObjects.size());

            for (gameObject* obj : Scene->GameObjects){
                for (auto& com : obj->Components){
                    if (com.second)DrawOrder.push_back(Internal::renderCache{com.second, com.second->RenderLayer});
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
            auto copy = DrawOrder;
            for (auto& renderer : copy){
                renderer.com->Draw(Win);
            }

            if (Win->Debug){
                auto copy = DrawOrder;
                //loop through all components and Draw them
                for (auto& renderer : copy){
                    renderer.com->DebugDraw(Win);
                }
            }
        }

    }// RenderWorldObjects()

    //Render all renderable objects
    void Render(){
        auto windows = Internal::GlobalWindows;  // Make a copy

        for (window* Win : windows){
            RenderDefaultWorld(Win);
            RenderWorldObjects(Win);
            RenderCanvases(Win);

            //present all renderers
            SDL_RenderPresent(Win->SDLrenderer);
        }
    }


} //obj