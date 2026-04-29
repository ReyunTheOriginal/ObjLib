#pragma once

#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <unordered_map>

#include "GlobalLists.hpp"
#include "Window.hpp"

namespace obj{
    struct sprite{
        private:
        std::string SpritePath = "";

        int ID = 0;

        friend sprite* CreateSprite(std::string ImagePath); //give it private acess
        friend std::string GetImagePath(); //give it private acess

        public:
        SDL_Surface* SDLsurface = nullptr;

        std::unordered_map<SDL_Renderer*, SDL_Texture*> Textures;

        std::string GetImagePath(){return SpritePath;}
        int GetID(){return ID;}
    };

    sprite* CreateSprite(std::string ImagePath); //safely create the sprite
    void DestroySprite(sprite* sprite); //safely destroy the sprite

}