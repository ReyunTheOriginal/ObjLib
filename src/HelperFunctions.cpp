#include "HelperFunctions.hpp"
#include "Objects.hpp"


#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

std::random_device rnd;
std::mt19937 gen(rnd());

namespace obl{

window* CreateWindow(std::string Title, vector2 Resulotion, Uint32 SDL_Window_Flags, Uint32 SDL_Renderer_Flags){
    SDL_Window* NewWindow = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)Resulotion.x, (int)Resulotion.y, SDL_Window_Flags);
    SDL_Renderer* NewRenderer = SDL_CreateRenderer(NewWindow, -1, SDL_Renderer_Flags);

    SDL_ShowWindow(NewWindow);

    std::shared_ptr<window> New = std::make_shared<window>();
    New->SDLRenderer = NewRenderer;
    New->SDLWindow = NewWindow;

    New->Cursor = new Internal::Cursor();
    New->Cursor->Init(New.get());

    New->Cameras.push_back(new camera());
    New->ActiveCamera = New->Cameras.back();

    Windows.push_back(New);

    return New.get();
}
gameObject* CreateGameObject(window& Window){

    std::shared_ptr<gameObject> New = std::make_shared<gameObject>();

    New->Name = "GameObject(" + std::to_string(New->GetID()) + ")";
    New->Transform = std::make_unique<Internal::transform>();
    New->Transform->Init(New.get());

    New->Transform->Window = &Window;
    New->Transform->Window->WindowObjects.push_back(New);

    return New.get();
}

std::string Lower(std::string stri){
    std::transform(stri.begin(), stri.end(), stri.begin(), ::tolower);
    return stri;
}
std::string Upper(std::string stri){
    std::transform(stri.begin(), stri.end(), stri.begin(), ::toupper);
    return stri;
}

std::string exePath(){
    namespace fs = std::filesystem;
    #ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return fs::path(buffer).parent_path().string();
    #elif __linux__
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            return fs::path(buffer).parent_path().string();
        }
    #elif __APPLE__
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            return fs::path(buffer).parent_path().string();
        }
    #endif

    return fs::current_path().string();
}

}//obj
