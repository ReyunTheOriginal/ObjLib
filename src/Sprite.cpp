#include "Sprite.hpp"

namespace obj{
    //get the set image path
    std::string sprite::GetImagePath(){
        return this->SpritePath;
    }
    
    //safely create the sprite
    sprite* CreateSprite(std::string ImagePath){
        if (std::filesystem::exists(ImagePath)){
            sprite* ref = new sprite();

            ref->SDLsurface = IMG_Load(ImagePath.c_str());
            ref->SpritePath = ImagePath;

            return ref;
        }else{
            return nullptr;
        }
    }
    //safely destroy the sprite
    void DestroySprite(sprite* sprite){
        delete sprite;
    }
}