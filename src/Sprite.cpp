#include "Sprite.hpp"

namespace obj{    
    //safely create the sprite
    sprite* CreateSprite(std::string ImagePath){
        if (std::filesystem::exists(ImagePath)){
            sprite* newSprite = new sprite();
            
            Internal::GlobalSprites.push_back(newSprite);
            newSprite->ID = Internal::Spr_ID;
            Internal::Spr_ID++;

            newSprite->SDLsurface = IMG_Load(ImagePath.c_str());
            newSprite->SpritePath = ImagePath;

            for (window* Win : Internal::GlobalWindows){
                newSprite->Textures[Win->SDLrenderer] = SDL_CreateTextureFromSurface(Win->SDLrenderer,newSprite->SDLsurface);
            }

            return newSprite;
        }else{
            std::cout << "Sprite not Found:" << '"'<< ImagePath << '"' << "\n";
            return nullptr;
        }
    }
    //safely destroy the sprite
    void DestroySprite(sprite* sprite){
        if (sprite->SDLsurface)SDL_DestroySurface(sprite->SDLsurface);

        for (auto& tex : sprite->Textures){
            SDL_DestroyTexture(tex.second);
        }
        
        delete sprite;
    }
}