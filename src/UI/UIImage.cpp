#include "UI/UIImage.hpp"

#include "UI/Canvas.hpp"
#include <SDL3/SDL.h>
#include "Sprite.hpp"
#include "Camera/Camera.hpp"
#include "UI/ScreenObject.hpp"
#include "UI/UITransform.hpp"

namespace obj{

    namespace UI{
        void image::Draw(window* Window){
            if (Window->GetSDLRenderer() && GetScreenObject()->GetCanvas() != nullptr && Sprite && Sprite->Textures.contains(Window->GetSDLRenderer())){
                auto tex = Sprite->Textures[Window->GetSDLRenderer()];
                if (tex){
                    //get texture vectors
                    float w, h;
                    SDL_GetTextureSize(tex, &w, &h);

                    camera* ActiveCamera = GetScreenObject()->GetCanvas()->GetCamera();
                    
                    if (!ActiveCamera) return;

                    //scale the rect
                    float zoom = ActiveCamera->Zoom;
                    float scaledH = h * GetScreenObject()->UITransform->GetScreenScale().y;
                    float scaledW = w * GetScreenObject()->UITransform->GetScreenScale().x;

                    SDL_FRect dst = {GetScreenObject()->UITransform->GetScreenPosition().x - scaledW * Pivot.Point.x , 
                        GetScreenObject()->UITransform->GetScreenPosition().y - scaledH * Pivot.Point.y, scaledW, scaledH };

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
                        GetScreenObject()->UITransform->GetScreenRotation(),
                        NULL,   // center (NULL = center of dst)
                        Flip
                    );
                }
            }
        }
    }

}