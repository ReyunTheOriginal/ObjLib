#include "Window.hpp"
#include "Scene.hpp"
#include <algorithm>
#include "Sprite.hpp"
#include "Camera.hpp"

namespace obj{
    window::window() = default;

    void window::SetScene(scene* SceneToSet){
        Scene = SceneToSet;
        // Add window to scene if not already present
        if (std::find(SceneToSet->Windows.begin(), SceneToSet->Windows.end(), this) == SceneToSet->Windows.end()){
            SceneToSet->Windows.push_back(this);
            vector2 res = SceneToSet->ActiveCamera->GetResolution();
            SDL_SetRenderLogicalPresentation(SDLrenderer, res.x, res.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        }
    }

    vector2 window::SetResolution(const vector2& res){
        SDL_SetWindowSize(SDLwindow, res.x, res.y);
        CachedResolution = res;
        return res;
    }

    window* CreateWindow(std::string title, obj::vector2 resolution, Uint64 WindowFlags){
        //create a new window
        window* newWin = new window();
        newWin->SDLwindow = SDL_CreateWindow(title.c_str(), (int)resolution.x, (int)resolution.y, WindowFlags);
        newWin->SDLrenderer = SDL_CreateRenderer(newWin->SDLwindow, NULL);
        newWin->CachedResolution = resolution;

        //add the window to the global list and increase the ID
        newWin->ID = Internal::Win_ID;
        Internal::GlobalWindows.push_back(newWin);
        Internal::Win_ID++;

        for (sprite* spri : Internal::GlobalSprites){
            if (!spri->Textures.contains(newWin->SDLrenderer)){
                spri->Textures[newWin->SDLrenderer] = SDL_CreateTextureFromSurface(newWin->SDLrenderer,spri->SDLsurface);
            }
        }

        return newWin;  // Returns a copy/move; no dangling reference
    }

    void DestroyWindow(window* Window){
        if (Window){
            delete Window;
        }
    }
}