#include "Rendering/Rendering.hpp"
#include <iostream>
#include <algorithm>


#include "Component.hpp"
#include "Rendering/SDLRenderer.hpp"
#include "GlobalTypes.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Camera/Camera.hpp"
#include "UI/UIBase.hpp"
#include "Components/Transform.hpp"


namespace obj{
    
    namespace Internal{
        struct renderCache{
            ::obj::component* com = nullptr;
            renderSorter* Layer = nullptr;
        };

        struct screenRenderCache{
            UI::screenComponent* com = nullptr;
            renderSorter* Layer = nullptr;
        };

        renderer* Renderer = nullptr;
    } //Internal

    void AddChildrenToUIDraw(std::vector<Internal::screenRenderCache>& List, UI::Internal::transformUI* transform){
        if (!transform || !transform->GetScreenObject() || !transform->GetScreenObject()->Enabled) return;
        
        UI::screenObject* obj = transform->GetScreenObject();
        
        auto componentsCopy = obj->Components;
        
        for (auto& comp : componentsCopy){
            if (comp.second && comp.second->Enabled && comp.second->RenderLayer){
                List.push_back(Internal::screenRenderCache{comp.second, comp.second->RenderLayer});
            }
        }

        for (UI::Internal::transformUI* child : transform->GetChildren())
            AddChildrenToUIDraw(List, child);
    }

    void AddChildrenToDraw(std::vector<Internal::renderCache>& List, Internal::transform* transform){
        if (!transform || !transform->GetGameObject() || !transform->GetGameObject()->Enabled) return;
        
        gameObject* obj = transform->GetGameObject();
        
        auto componentsCopy = obj->Components;
        
        for (auto& comp : componentsCopy){
            if (comp.second && comp.second->Enabled && comp.second->RenderLayer){
                List.push_back(Internal::renderCache{comp.second, comp.second->RenderLayer});
            }
        }

        for (Internal::transform* child : transform->GetChildren()){
            AddChildrenToDraw(List, child);
        }
    }

    void RenderCanvases(window* Win){
        if (!Win)return;

        scene* Scene = Win->GetScene();
        if (!Scene) return;

        camera* ActiveCamera = Win->GetCamera();
        if (!ActiveCamera)return;

        UI::canvas* Canvas = ActiveCamera->GetCanvas();
        if (!Canvas)return;

        std::vector<Internal::screenRenderCache> DrawOrder;
        DrawOrder.reserve(Canvas->GetScreenObjects().size());

        for (UI::screenObject* obj : Canvas->GetParentlessScreenObjects()){
            AddChildrenToUIDraw(DrawOrder, obj->UITransform);
        }

        std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::screenRenderCache& A, const Internal::screenRenderCache& B){
            if (A.Layer->Layer != B.Layer->Layer) return A.Layer->Layer < B.Layer->Layer;
            if (A.Layer->Order != B.Layer->Order) return A.Layer->Order < B.Layer->Order;
            return A.com->GetScreenObject()->GetID() < B.com->GetScreenObject()->GetID();
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
        Internal::Renderer->SetDrawColor(Win, {0, 0, 0, 255});
        Internal::Renderer->Clear(Win);

        scene* Scene = Win->GetScene();
        if (!Scene) return;

        camera* ActiveCamera = Win->GetCamera();
        
        if (ActiveCamera){
            vector2 Logical = Internal::Renderer->GetResolution(Win);

            //Draw the background first<
            color bgColor = Scene->BackGroundColor;

            Internal::Renderer->SetDrawColor(Win,
                {bgColor.r,
                bgColor.g,
                bgColor.b,
                255}
            );

            rect Rect = {0, 0, Logical.x, Logical.y};
            Internal::Renderer->FillRect(Win, Rect);
        }

    }

    void RenderWorldObjects(window* Win){
        if (!Win)return;

        scene* Scene = Win->GetScene();

        if (!Scene) return;

        camera* ActiveCamera = Win->GetCamera();
        
        if (ActiveCamera){
            std::vector<Internal::renderCache> DrawOrder;
            DrawOrder.reserve(Scene->GetGameObjects().size());

            for (gameObject* obj : Scene->GetParentlessGameObjects()){
                AddChildrenToDraw(DrawOrder, obj->Transform);
            }

           std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::renderCache& A, const Internal::renderCache& B){
                if (A.Layer->Layer != B.Layer->Layer) return A.Layer->Layer < B.Layer->Layer;
                if (A.Layer->Order != B.Layer->Order) return A.Layer->Order < B.Layer->Order;
                return A.com->GetGameObject()->GetID() < B.com->GetGameObject()->GetID();
            });

            //loop through all components and Draw them
            for (const auto& com : DrawOrder){
                com.com->Draw(Win);
            }

            if (Win->Debug){
                //loop through all components and Draw them
                for (const auto& com : DrawOrder){
                    com.com->DebugDraw(Win);
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
            Internal::Renderer->Present(Win);
        }
    }


} //obj