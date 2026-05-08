#pragma once

#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

#include "GlobalLists.hpp"
#include "Window.hpp"

namespace obj{
    struct sprite{
        private:
        std::string SpritePath = "";

        int ID = 0;

        friend sprite* CreateSprite(std::string ImagePath);
        friend std::string GetImagePath();
        SDL_Surface* SDLsurface = nullptr;

        public:
        std::unordered_map<SDL_Renderer*, SDL_Texture*> Textures;

        SDL_Surface* GetSDLSurface(){return SDLsurface;}

        std::string GetImagePath(){return SpritePath;}
        int GetID(){return ID;}

        ~sprite(){
            if (SDLsurface)SDL_DestroySurface(SDLsurface);

            for (auto& tex : Textures){
                SDL_DestroyTexture(tex.second);
            }
        }
    };

    sprite* CreateSprite(std::string ImagePath); //safely create the sprite

}