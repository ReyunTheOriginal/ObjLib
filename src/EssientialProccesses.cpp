#include "EssientialProccesses.hpp"

#include "Core.hpp"


namespace obl{
    SDL_Cursor* CursorIMG = nullptr;

    void Init(){
        //start everything
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);

        namespace fs = std::filesystem;
        //get sprites from a Sprites folder, if it doesn't exist make it
        if (!fs::exists(exePath() + "/Sprites")) fs::create_directory(exePath() + "/Sprites");
        for (auto& file : fs::recursive_directory_iterator(exePath() + "/Sprites")){
            std::string exten = file.path().extension().string();

            std::string low = Lower(exten);

            if (low == ".png" || low == ".jpg" || low == ".jpeg"){
                std::string name = file.path().stem().string();
                std::string path = file.path().string();
                Sprites[name] = path;
            }
        }

        //get sprites from a Fonts folder, if it doesn't exist make it
        if (!fs::exists(exePath() + "/Fonts")) fs::create_directory(exePath() + "/Fonts");
        for (auto& file : fs::recursive_directory_iterator(exePath() + "/Fonts")){
            std::string exten = file.path().extension().string();

            std::string low = Lower(exten);

            if (low == ".ttf" || low == ".otf"){
                std::string name = file.path().stem().string();
                std::string path = file.path().string();
                Fonts[name] = path;
            }
        }
        
        //Set Custom Cursor
        if (!Sprites.contains("Cursor")){
            Print("no 'Cursor' Image Found in /Sprites");
        }else{
            SDL_Surface* surface = IMG_Load(Sprites["Cursor"].c_str());
            if (surface){
                int ScaledWidth = surface->w * 0.2;
                int ScaledHeight = surface->h * 0.2;

                SDL_Surface* ScaledSurface = SDL_CreateRGBSurfaceWithFormat(0, ScaledWidth, ScaledHeight, 32, surface->format->format);
                SDL_BlitScaled(surface, NULL, ScaledSurface, NULL);

                CursorIMG = SDL_CreateColorCursor(ScaledSurface, 0,0);
                SDL_SetCursor(CursorIMG);
                SDL_FreeSurface(ScaledSurface);
                SDL_FreeSurface(surface);
            }else{
                Print("Failed To Create Cursor Surface");
            }
        }
    }

    //update all engine processes like gameobject possitions & stuff
    void Update(){
        Input.Update();
        Time.Update();
        Frames.Update();

        for (auto& win : Windows){
            win->RenderableObjects.clear();
            win->Update();
            win->Cursor->Update();
        }

    }

    void Quit(){
        if (CursorIMG)SDL_FreeCursor(CursorIMG);

        Windows.clear();

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        exit(0);
    }

}