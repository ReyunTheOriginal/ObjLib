#include "Window.hpp"

namespace obj{
    window::window() = default;

    window* CreateWindow(std::string title, obj::vector2 resolution, Uint64 WindowFlags){
        //create a new window
        window* newWin = new window();
        newWin->SDLwindow = SDL_CreateWindow(title.c_str(), (int)resolution.x, (int)resolution.y, WindowFlags);
        newWin->SDLrenderer = SDL_CreateRenderer(newWin->SDLwindow, NULL);

        //add the window to the global list and increase the ID
        Internal::WID++;
        Internal::GlobalWindows[Internal::WID] = newWin;

        return newWin;  // Returns a copy/move; no dangling reference
    }

    void DestroyWindow(window* Window){
        if (Window){
            delete Window;
        }
    }
}