#include "Components/SpriteRenderer.hpp"

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "Components/Transform.hpp"
#include "EssentialProcesses.hpp"

#include <unordered_map>

namespace obj{

    void spriteRenderer::Draw(window* Window){
        if (Window->SDLrenderer && GameObject && GameObject->GetScene() && Sprite && Sprite->Textures.contains(Window->SDLrenderer)){
            auto tex = Sprite->Textures[Window->SDLrenderer];
            if (tex){
                camera* ActiveCamera = Window->ActiveCamera;

                //scale the rect
                float zoom = ActiveCamera->Zoom;
                float scaledH = (GameObject->Transform->GetWorldScale().y * PixelsPerUnit) * zoom;
                float scaledW = (GameObject->Transform->GetWorldScale().x * PixelsPerUnit) * zoom;

                vector2 ScreenPos = ActiveCamera->WorldToScreenPosition(GameObject->Transform->GetWorldPosition());

                SDL_FRect dst = {ScreenPos.x - scaledW / 2.0f , 
                    ScreenPos.y - scaledH / 2.0f, scaledW, scaledH };

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
                    Window->SDLrenderer,
                    tex,
                    NULL,   // src rect (whole texture)
                    &dst,   // dst rect
                    -(GameObject->Transform->GetWorldRotation() + ActiveCamera->Rotation),
                    NULL,   // center (NULL = center of dst)
                    Flip
                );
            }
        }
    }
}