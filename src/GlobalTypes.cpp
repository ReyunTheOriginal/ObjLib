#include "Core.hpp"

namespace obl{
    #pragma region <Window Functions>
        std::string window::DebugifyTitle(std::string New = ""){
            if (New.empty())New = Title;

            New = New + std::format(" -Debug( {{Res: ({}, {})}}, {{FPS: {}}}, {{Object Count: {}}}, {{Camera Pos: ({:.2f}, {:.2f})}})", GetResulotion().x, GetResulotion().y, Frames.FPS, WindowObjects.size(),ActiveCamera->Position.x, ActiveCamera->Position.y);

            //Window Title -Debug( {Res: (800, 600)}, {FPS: 60}, {Object Count: 9}, {Camera Pos: (0, 0)})
            return New;
        }

        vector2 window::GetResulotion(){
            int x,y;
            SDL_GetWindowSize(SDLWindow,&x, &y);

            return {(float)x,(float)y};
        }

        void window::SetTitle(std::string newTitle){
            std::string OldTitle = Title;
            std::string OldDebugTitle = DebugifyTitle(Title);

            if (Debug == false){
                if (OldTitle != newTitle || SDL_GetWindowTitle(SDLWindow) == OldDebugTitle){
                    Title = newTitle;
                    DebugTitle = DebugifyTitle(Title);
                    SDL_SetWindowTitle(SDLWindow, newTitle.c_str());
                }
            }else{
                if (OldDebugTitle != DebugTitle || SDL_GetWindowTitle(SDLWindow) == OldTitle){
                    Title = newTitle;
                    DebugTitle = DebugifyTitle(Title);
                    SDL_SetWindowTitle(SDLWindow, DebugTitle.c_str());
                }
            }
        }

        void window::Update(){
            SetTitle(Title);

            for(auto& obj : WindowObjects){
                for(auto& com : obj->Components){
                    if (obj->Activated && com.second->Enabled)com.second->Run();
                }
                for(auto& child : obj->Transform->Children){
                    child->Transform->Parent = obj.get();
                }
            }
        }

        vector2 window::ScreenToWorldPosition(const vector2& ScreenPos){
            int WindowX, WindowY;
            SDL_GetWindowSize(SDLWindow, &WindowX, &WindowY);

            float Z = (ActiveCamera->GetZoom() != 0) ? ActiveCamera->GetZoom() : 1;

            vector2 cameraReletive;
                cameraReletive.x = (ScreenPos.x - (WindowX * 0.5)) / (Z * PixelsPerUnit);
                cameraReletive.y = (ScreenPos.y - (WindowY * 0.5)) / (Z * PixelsPerUnit);

            float sin = sinf((ActiveCamera->Rotation) * (Math::PI / 180.0f));
            float cos = cosf((ActiveCamera->Rotation) * (Math::PI / 180.0f));

            vector2 Rotated = {
                cameraReletive.x * cos - cameraReletive.y * sin,
                cameraReletive.x * sin + cameraReletive.y * cos
            };

            vector2 world = Rotated + ActiveCamera->Position;
            return world;
        }
        vector2 window::WorldToScreenPosition(const vector2& WorldPos){
            int WindowX, WindowY;
            SDL_GetWindowSize(SDLWindow, &WindowX, &WindowY);

            float Z = (ActiveCamera->GetZoom() != 0) ? ActiveCamera->GetZoom() : 1;

            vector2 cameraReletive = {
                WorldPos.x - ActiveCamera->Position.x,
                WorldPos.y - ActiveCamera->Position.y
            };

            float sin = sinf((-ActiveCamera->Rotation) * (Math::PI / 180.0f));
            float cos = cosf((-ActiveCamera->Rotation) * (Math::PI / 180.0f));

            cameraReletive *= Z * PixelsPerUnit;

            vector2 Rotated = {
                cameraReletive.x * cos - cameraReletive.y * sin,
                cameraReletive.x * sin + cameraReletive.y * cos
            };

            vector2 screen;
            screen.x = Rotated.x + (WindowX * 0.5f);
            screen.y = Rotated.y + (WindowY * 0.5f);
            return screen;
        }
    
    #pragma endregion
}