#include "Window.hpp"
#include "Scene.hpp"

namespace obj{
    window::window() = default;

    void window::SetScene(scene* SceneToSet){
        Scene = SceneToSet;
        if (SceneToSet->Windows.find(this) == SceneToSet->Windows.end()){
            SceneToSet->Windows.insert(this);
        }
    }

    vector2 window::SetResolution(const vector2& res){
        SDL_SetWindowSize(SDLwindow, res.x, res.y);
        return res;
    }

    window* CreateWindow(std::string title, obj::vector2 resolution, Uint64 WindowFlags){
        //create a new window
        window* newWin = new window();
        newWin->SDLwindow = SDL_CreateWindow(title.c_str(), (int)resolution.x, (int)resolution.y, WindowFlags);
        newWin->SDLrenderer = SDL_CreateRenderer(newWin->SDLwindow, NULL);

        //add the window to the global list and increase the ID
        Internal::GlobalWindows[Internal::WID] = newWin;
        newWin->ID = Internal::WID;
        Internal::WID++;

        return newWin;  // Returns a copy/move; no dangling reference
    }

    void DestroyWindow(window* Window){
        if (Window){
            delete Window;
        }
    }
}