#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

namespace obj{
    struct sprite{
        private:
        std::string SpritePath = "";

        friend sprite* CreateSprite(std::string ImagePath); //give it private acess
        friend std::string GetImagePath(); //give it private acess

        public:
        SDL_Surface* SDLsurface = nullptr;

        std::string GetImagePath(); //get the set image path

        ~sprite(){if (SDLsurface)SDL_DestroySurface(SDLsurface);}
    };

    sprite* CreateSprite(std::string ImagePath); //safely create the sprite
    void DestroySprite(sprite* sprite); //safely destroy the sprite

}