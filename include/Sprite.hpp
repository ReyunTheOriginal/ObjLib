#pragma once

#include <string>
#include <iostream>
#include "Rendering/Rendering.hpp"
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

#include "GlobalLists.hpp"

namespace obj{
    struct window;
    
    struct sprite{
        private:
        std::string SpritePath = "";

        int ID = 0;

        friend sprite* CreateSprite(std::string ImagePath);
        friend std::string GetImagePath();
        SDL_Surface* SDLsurface = nullptr;

        public:
        std::unordered_map<window*, texture*> Textures;

        SDL_Surface* GetSDLSurface(){return SDLsurface;}

        std::string GetImagePath(){return SpritePath;}
        int GetID(){return ID;}

        void CreateTextures();

        void CreateTextureForWindow(window* Window);

        ~sprite(){
            if (SDLsurface)SDL_DestroySurface(SDLsurface);

            for (auto& tex : Textures)
                delete tex.second;

            Textures.clear();
        }
    };

    sprite* CreateSprite(std::string ImagePath); //safely create the sprite

}