#include "Window.hpp"
#include "Scene.hpp"
#include <algorithm>
#include "Sprite.hpp"
#include "Camera/Camera.hpp"
#include <format>
#include "Framerate.hpp"
#include "Rendering/Rendering.hpp"

namespace obj{
    window::window() = default;

    void window::OnSceneChange(){}

    std::string window::SetTitle(std::string NewTitle){
        if (NewTitle != CachedTitle){
            SDL_SetWindowTitle(SDLwindow, NewTitle.c_str());
            CachedTitle = NewTitle;
        }
        return NewTitle;
    }
    std::string window::GetTitle(){
        return CachedTitle;
    }

    void window::DebugDisplay(){
        std::string result = CachedTitle;

        if (Scene && ActiveCamera){
            result = std::format(
                "'{}' Debug: (FPS: {} | Scene: {} | Object Count: {} | Camera Position: ({:.2f}, {:.2f} | Camera Zoom: {}))",
                CachedTitle,
                FPS::FPS,
                Scene->Name,
                Scene->GameObjects.size(),
                ActiveCamera->Position.x,
                ActiveCamera->Position.y,
                ActiveCamera->Zoom
            );
        }else if (!Scene && !ActiveCamera){
            result = std::format(
                "'{}' Debug: (FPS: {} | Has no Scene | Has no Camera))",
                CachedTitle,
                FPS::FPS
            );
        }else if (!Scene){
            result = std::format(
                "'{}' Debug: (FPS: {} | Has no Scene | Camera Position: ({:.2f}, {:.2f} | Camera Zoom: {}))",
                CachedTitle,
                FPS::FPS,
                ActiveCamera->Position.x,
                ActiveCamera->Position.y,
                ActiveCamera->Zoom
            );
        }else if (!ActiveCamera){
            result = std::format(
                "'{}' Debug: (FPS: {} | Scene: {} | Object Count: {} | Has no Camera))",
                CachedTitle,
                FPS::FPS,
                Scene->Name,
                Scene->GameObjects.size()
            );
        }

        SDL_SetWindowTitle(SDLwindow, result.c_str());
    }

    scene* window::SetScene(scene* SceneToSet){
        if (Scene && Scene != SceneToSet){
            std::erase(Scene->Windows, this);

            if (Scene->Windows.size() == 0)
                Scene->OnSceneUnLoad();
            
            Scene->OnSceneUnSet();
        }

        if (SceneToSet && SceneToSet != Scene){
            if (SceneToSet->Windows.size() == 0)
                SceneToSet->OnSceneLoad();
            

            if (std::find(SceneToSet->Windows.begin(), SceneToSet->Windows.end(), this) == SceneToSet->Windows.end())
                SceneToSet->Windows.push_back(this);

            SceneToSet->OnSceneSet();
        }

        Scene = SceneToSet;
        if (SceneToSet != Scene)OnSceneChange();
        return SceneToSet;
    }

    vector2 window::SetResolution(const vector2& res){
        SDL_SetWindowSize(SDLwindow, res.x, res.y);
        CachedResolution = res;
        return res;
    }
    
    camera* window::SetCamera(camera* Camera){
        if (!Camera) return nullptr;
        Camera->ActiveWindow = this;
        return Camera;
    }

    window::~window(){
            std::erase(Internal::GlobalWindows, this);
            
            if (Scene){
                if (Scene)std::erase(Scene->Windows, this);
            }

            if (SDLwindow)SDL_DestroyWindow(this->SDLwindow);
        };

    window* CreateWindow(std::string title, obj::vector2 resolution, SDL_WindowFlags flags){
        //create a new window
        window* newWin = new window();
        newWin->CachedTitle = title;
        newWin->ActiveCamera = CreateCamera(newWin);
        newWin->SDLwindow = SDL_CreateWindow(title.c_str(), (int)resolution.x, (int)resolution.y, flags | Internal::Renderer->GetWindowCreationFlag());
        Internal::Renderer->OnWindowCreation(newWin);
        newWin->CachedResolution = resolution;

        newWin->ActiveCamera->ActiveWindow = newWin;
        Internal::Renderer->SetResolution(newWin, resolution);

        auto sprites = Internal::GlobalSprites;

        for (sprite* Sprite : sprites)
            Sprite->CreateTextureForWindow(newWin);

        //add the window to the global list and increase the ID
        newWin->ID = Internal::Obj_ID;
        Internal::GlobalWindows.push_back(newWin);
        Internal::Obj_ID++;

        return newWin;  // Returns a copy/move; no dangling reference
    }
}