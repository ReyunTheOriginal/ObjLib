#include "Sprite.hpp"
#include <filesystem>

namespace obj{    
    //safely create the sprite
    sprite* CreateSprite(std::string ImagePath){
        if (std::filesystem::exists(ImagePath)){
            sprite* newSprite = new sprite();
            
            Internal::GlobalSprites.push_back(newSprite);
            newSprite->ID = Internal::Obj_ID;
            Internal::Obj_ID++;

            newSprite->SDLsurface = IMG_Load(ImagePath.c_str());
            newSprite->SpritePath = ImagePath;

            auto windows = Internal::GlobalWindows;  // Make a copy
            for (window* Win : windows){
                newSprite->Textures[Win->SDLrenderer] = SDL_CreateTextureFromSurface(Win->SDLrenderer,newSprite->SDLsurface);
            }

            return newSprite;
        }else{
            std::cout << "Sprite not Found:" << '"'<< ImagePath << '"' << "\n";
            return nullptr;
        }
    }
}