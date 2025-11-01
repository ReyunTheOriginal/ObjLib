#include "Rendering.hpp"

namespace obl{
    namespace components{
        #pragma region <SpriteRenderer>
            void SpriteRenderer::CreateTexture(){
                if (Texture) SDL_DestroyTexture(Texture);
                SDL_Texture* tex = IMG_LoadTexture(GameObject->Transform->Window->SDLRenderer, Sprites[Sprite].c_str());
                Texture = tex;
            }

            std::string SpriteRenderer::GetSprite(){
                return Sprite;
            }
            void SpriteRenderer::SetSprite(std::string NewSprite){
                Sprite = NewSprite;
                CreateTexture();
            }

            void SpriteRenderer::Run(){
                if (Drawn){
                    if (GameObject->GetComponent<renderLayer>()){
                        if (Texture){
                            GameObject->Transform->Window->RenderableObjects.push_back(GameObject);
                        }else{
                            if (Sprites.contains(Sprite)){
                                CreateTexture();
                            }else{
                                Print("Sprite Doesn't Exist in the Sprites Folder, check spelling. (remember sprites use their file name without the file extention)");
                                return;
                            }

                            Print("Texture Not Found for " + GameObject->Name);
                        }//if (Texture)
                    }else{
                        Print("Object Has Renderer But No <RenderLayer>");
                    }//if (renderlayer)
                }//if (Drawn)
            }//Run()

            void SpriteRenderer::Draw(){
                int X, Y;
                SDL_QueryTexture(Texture, nullptr, nullptr, &X, &Y);

                SDL_SetTextureColorMod(Texture, Color.red, Color.green, Color.blue);
                SDL_SetTextureAlphaMod(Texture, Color.alpha);

                vector2 ScreenPos = GameObject->Transform->Window->WorldToScreenPosition(GameObject->Transform->Position);

                SDL_FRect rect;
                rect.w = ((X * GameObject->Transform->Size) * GameObject->Transform->Window->ActiveCamera->GetZoom());
                rect.h = ((Y * GameObject->Transform->Size) * GameObject->Transform->Window->ActiveCamera->GetZoom());
                rect.x = (ScreenPos.x - rect.w / 2);
                rect.y = (ScreenPos.y - rect.h / 2);

                SDL_FPoint center = {rect.w / 2, rect.h / 2};

                SDL_RendererFlip flip = SDL_FLIP_NONE;
                if (FlippedX && FlippedY)
                    flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
                else if (FlippedX)
                    flip = SDL_FLIP_HORIZONTAL;
                else if (FlippedY)
                    flip = SDL_FLIP_VERTICAL;

                SDL_RenderCopyExF(GameObject->Transform->Window->SDLRenderer, Texture, nullptr, &rect, GameObject->Transform->Rotation - GameObject->Transform->Window->ActiveCamera->Rotation, &center, flip);
            }//Draw()
        #pragma endregion
    
        #pragma region <renderLayer>
            void renderLayer::Run(){}
        #pragma endregion
    }//Components
}//obl

/*

*/