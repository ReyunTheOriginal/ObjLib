#include "UI/Image.hpp"

#include "UI/Canvas.hpp"
#include <SDL3/SDL.h>
#include "Sprite.hpp"
#include "Camera/Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/UITransform.hpp"
#include "HelperFunctions.hpp"

namespace obj{

    namespace UI{
        void image::Draw(window* Window){
            if (Window && GetScreenObject()->GetCanvas() != nullptr && Sprite){
                auto Texture = Sprite->Textures[Window];
                if (Texture){
                    //set the texture colors
                    Texture->SetTextureColorMod(Color);

                    SDL_FlipMode Flip = SDL_FLIP_NONE;
                    if (FlipHorizontal){
                        Flip = SDL_FLIP_HORIZONTAL;
                        if (FlipVertical){
                            Flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
                        }
                    }else if (FlipVertical){
                        Flip = SDL_FLIP_VERTICAL;
                        if (FlipHorizontal){
                            Flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
                        }
                    }
                    
                    //render it rotated as necessary
                    vector2 scaledDimensions = {Texture->GetSize().x * GetScreenObject()->UITransform->GetScreenScale().x,
                                                Texture->GetSize().y * GetScreenObject()->UITransform->GetScreenScale().y};
                    ::obj::Internal::Renderer->DrawSpriteRotated(
                        Window, 
                        Sprite, 
                        GetScreenObject()->UITransform->GetScreenPosition(), 
                        scaledDimensions, 
                        GetScreenObject()->UITransform->GetScreenRotation(),
                        Pivot,
                        Flip
                    );
                    
                    Print(Pivot.Point);
                }else{
                    std::cout << "Sprite Has No Texture for:" + GetScreenObject()->Name << "\n";
                }
            }
        }
    }

}