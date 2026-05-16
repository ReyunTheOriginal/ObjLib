#include "Sprite.hpp"
#include <filesystem>
#include "Rendering/Rendering.hpp"

namespace obj{    
    //safely create the sprite
    sprite* CreateSprite(std::string ImagePath){
        SDL_Surface* surf = Internal::Renderer->LoadSurfaceFromImage(ImagePath);
        if (!surf) return nullptr;

        sprite* newSprite = new sprite();
        
        Internal::GlobalSprites.push_back(newSprite);
        newSprite->ID = Internal::Obj_ID;
        Internal::Obj_ID++;

        newSprite->SDLsurface = IMG_Load(ImagePath.c_str());
        newSprite->SpritePath = ImagePath;

        newSprite->CreateTextures();

        return newSprite;
    }

    void sprite::CreateTextures(){
        if (!SDLsurface) return;

        auto windows = Internal::GlobalWindows;  // Make a copy
        for (window* Win : windows){
            if (!Textures.contains(Win))
                Textures[Win] = Internal::Renderer->CreateTextureFromSurface(Win, SDLsurface);
        }
    }

    void sprite::CreateTextureForWindow(window* Window){
        if (!SDLsurface) return;

        if (!Textures.contains(Window))
            Textures[Window] = Internal::Renderer->CreateTextureFromSurface(Window, SDLsurface);
    }
}