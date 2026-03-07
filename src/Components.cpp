#include "Components.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Gameobject.hpp"
#include <iostream>

namespace obj{
    namespace Internal{

    };

    #pragma region <Components>
/////////////////////////////////////////////////////////// 
        #pragma region <spriteRenderer>
            void spriteRenderer::Run(){
                if (SDLtexture == nullptr && GameObject != nullptr && GameObject->Scene != nullptr && GameObject->Scene->Window != nullptr && GameObject->Scene->Window->SDLrenderer != nullptr){
                    if (Sprite){
                        SDLtexture = SDL_CreateTextureFromSurface(GameObject->Scene->Window->SDLrenderer,Sprite->SDLsurface);
                    }
                }
            }
            void spriteRenderer::Draw(){
                if (SDLtexture != nullptr && Sprite != nullptr && GameObject != nullptr && GameObject->Scene != nullptr && GameObject->Scene->Window != nullptr && GameObject->Scene->Window->SDLrenderer != nullptr){
                    //get texture vectors
                    float w, h;
                    SDL_GetTextureSize(SDLtexture, &w, &h);
                    SDL_FRect dst = {GameObject->Position.x, GameObject->Position.y, w, h };

                    //render it rotated as necessary
                    SDL_RenderTextureRotated(
                        GameObject->Scene->Window->SDLrenderer,
                        SDLtexture,
                        NULL,   // src rect (whole texture)
                        &dst,   // dst rect
                        GameObject->Rotation,
                        NULL,   // center (NULL = center of dst)
                        SDL_FLIP_NONE //not flipped
                    );

                }else if (SDLtexture == nullptr && GameObject != nullptr && GameObject->Scene != nullptr && GameObject->Scene->Window != nullptr && GameObject->Scene->Window->SDLrenderer != nullptr){
                    if (Sprite){
                        SDLtexture = SDL_CreateTextureFromSurface(GameObject->Scene->Window->SDLrenderer,Sprite->SDLsurface);
                    }
                }
            }
            void spriteRenderer::Destroy(){
                if (Sprite) DestroySprite(Sprite);
                delete this;
            }
        #pragma endregion <spriteRenderer>
////////////////////////////////////////////////////////////
    #pragma endregion <Components>
}