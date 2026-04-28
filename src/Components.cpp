#include "Components.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include <iostream>

namespace obj{
    #pragma region <Components>
/////////////////////////////////////////////////////////// 
        #pragma region <spriteRenderer>
            void spriteRenderer::Run(){
                //loop through all windows with the scene and create a texture for them
                if (GameObject && GameObject->Scene){
                    for (window* Win : GameObject->Scene->Windows){
                        if (Win != nullptr && Win->SDLrenderer != nullptr && !SDLtextures[Win->SDLrenderer] && Sprite){
                            SDLtextures[Win->SDLrenderer] = SDL_CreateTextureFromSurface(Win->SDLrenderer,Sprite->SDLsurface);
                        }
                    }
                }
            }
            void spriteRenderer::Draw(SDL_Renderer* renderer){
                if (GameObject != nullptr && GameObject->Scene != nullptr){
                    auto tex = SDLtextures[renderer];
                    if (renderer != nullptr && tex && Sprite != nullptr){
                        //normal Draw
            
                        //get texture vectors
                        float w, h;
                        SDL_GetTextureSize(tex, &w, &h);

                        camera* ActiveCamera = GameObject->Scene->ActiveCamera;

                        //scale the rect
                        float zoom = ActiveCamera->Zoom;
                        float scaledH = h * GameObject->Size.y * zoom;
                        float scaledW = w * GameObject->Size.x * zoom;

                        vector2 ScreenPos = ActiveCamera->WorldToScreenPosition(GameObject->Position);

                        SDL_FRect dst = {ScreenPos.x - scaledW / 2.0f , 
                            ScreenPos.y - scaledH / 2.0f, scaledW, scaledH };

                        //set the texture colors
                        SDL_SetTextureColorMod(tex, Color.r, Color.g, Color.b);
                        SDL_SetTextureAlphaMod(tex, Color.a);

                        
                        //render it rotated as necessary
                        SDL_RenderTextureRotated(
                            renderer,
                            tex,
                            NULL,   // src rect (whole texture)
                            &dst,   // dst rect
                            GameObject->Rotation + ActiveCamera->Rotation,
                            NULL,   // center (NULL = center of dst)
                            SDL_FLIP_NONE //not flipped
                        );

                        //Debug Draw
                        DebugDraw();
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