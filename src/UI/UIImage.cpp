#include "UI/UIImage.hpp"

#include "UI/Canvas.hpp"
#include <SDL3/SDL.h>
#include "Sprite.hpp"
#include "Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/UITransform.hpp"

namespace obj{

    namespace UI{
        void image::Draw(window* Window){
            if (Window->GetSDLRenderer() && ScreenObject->GetCanvas() != nullptr && Sprite && Sprite->Textures.contains(Window->GetSDLRenderer())){
                auto tex = Sprite->Textures[Window->GetSDLRenderer()];
                if (tex){
                    //get texture vectors
                    float w, h;
                    SDL_GetTextureSize(tex, &w, &h);

                    camera* ActiveCamera = ScreenObject->GetCanvas()->GetCamera();
                    
                    if (!ActiveCamera) return;

                    //scale the rect
                    float zoom = ActiveCamera->Zoom;
                    float scaledH = h * ScreenObject->UITransform->GetScreenScale().y;
                    float scaledW = w * ScreenObject->UITransform->GetScreenScale().x;

                    SDL_FRect dst = {ScreenObject->UITransform->GetScreenPosition().x - scaledW / 2.0f , 
                        ScreenObject->UITransform->GetScreenPosition().y - scaledH / 2.0f, scaledW, scaledH };

                    //set the texture colors
                    SDL_SetTextureColorMod(tex, Color.r, Color.g, Color.b);
                    SDL_SetTextureAlphaMod(tex, Color.a);

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
                    SDL_RenderTextureRotated(
                        Window->GetSDLRenderer(),
                        tex,
                        NULL,   // src rect (whole texture)
                        &dst,   // dst rect
                        ScreenObject->UITransform->GetScreenRotation(),
                        NULL,   // center (NULL = center of dst)
                        Flip
                    );
                }
            }
        }
    }

}