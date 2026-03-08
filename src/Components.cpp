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
                //loop through all windows with the scene and create a texture for them
                if (GameObject && GameObject->Scene){
                    for (window* Win : GameObject->Scene->Windows){
                        if (Win != nullptr && Win->SDLrenderer != nullptr && !SDLtextures[Win->SDLrenderer] && Sprite){
                            SDLtextures[Win->SDLrenderer] = SDL_CreateTextureFromSurface(Win->SDLrenderer,Sprite->SDLsurface);
                        }
                    }
                }
            }
            void spriteRenderer::Draw(){
                if (GameObject != nullptr && GameObject->Scene != nullptr){
                    for (window* Win : GameObject->Scene->Windows){
                        auto tex = SDLtextures[Win->SDLrenderer];
                        if (Win->SDLrenderer != nullptr && tex && Sprite != nullptr){
                            //normal Draw
                
                            //get texture vectors
                            float w, h;
                            SDL_GetTextureSize(tex, &w, &h);
                            //scale the rect
                            float scaledH = h * GameObject->Size.y;
                            float scaledW = w * GameObject->Size.x;

                            SDL_FRect dst = {GameObject->Position.x - scaledW / 2.0f , GameObject->Position.y - scaledH / 2.0f, scaledW, scaledH };

                            //set the texture colors
                            SDL_SetTextureColorMod(tex, Color.r, Color.g, Color.b);
                            SDL_SetTextureAlphaMod(tex, Color.a);

                            
                            //render it rotated as necessary
                            SDL_RenderTextureRotated(
                                Win->SDLrenderer,
                                tex,
                                NULL,   // src rect (whole texture)
                                &dst,   // dst rect
                                GameObject->Rotation,
                                NULL,   // center (NULL = center of dst)
                                SDL_FLIP_NONE //not flipped
                            );

                            //Debug Draw
                            DebugDraw();
                        }


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