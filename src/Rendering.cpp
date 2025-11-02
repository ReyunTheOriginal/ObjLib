#include "Rendering.hpp"
#include "Components.hpp"

namespace obl{

namespace Internal{
    struct renderCache{
        gameObject* obj = nullptr;
        components::renderLayer* Layer = nullptr;
    };
}

void RenderWindows(){
    for(auto& win : Windows){
        std::vector<Internal::renderCache> DrawOrder;
        DrawOrder.reserve(win->RenderableObjects.size());

        for (auto& obj : win->RenderableObjects){
            DrawOrder.push_back(Internal::renderCache{obj.second, obj.second->GetConstComponent<components::renderLayer>()});
        }

        std::sort(DrawOrder.begin(), DrawOrder.end(), [](const Internal::renderCache& A, const Internal::renderCache& B){

            if (A.Layer->Layer == B.Layer->Layer && A.Layer->RenderOrder == B.Layer->RenderOrder){
                return A.obj->GetID() < B.obj->GetID();
            }else if (A.Layer->Layer == B.Layer->Layer){
                return A.Layer->RenderOrder < B.Layer->RenderOrder;
            }else{
                return A.Layer->Layer < B.Layer->Layer;
            }
        });

        for (auto& obj : DrawOrder){
            for (auto& com :obj.obj->Components){
                com.second->Draw();
            }
        }

        if (win->Debug){
            for (auto& obj : DrawOrder){
                for (auto& com :obj.obj->Components){
                    com.second->DebugDraw();
                }
            }
        }

        win->Cursor->Draw();
        SDL_RenderPresent(win->SDLRenderer);
    }
}

void ClearWindows(){
    for(auto& win : Windows){
        //clear frame if enabled
        if (win->ClearFrame){
            SDL_SetRenderDrawColor(
                win->SDLRenderer, 
                win->ClearColor.red,
                win->ClearColor.green,
                win->ClearColor.blue,
                win->ClearColor.alpha
            );

            SDL_RenderClear(win->SDLRenderer);
        }
    }
}

}//obj