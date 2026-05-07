#include "UI/Text.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "UI/UIBase.hpp"
#include "Camera.hpp"

namespace obj{
    namespace UI{

        void Text::SetText(std::string newText){
            if (newText == content) return;
            content = newText;
            RecreateTextures();
        }

        void Text::RecreateTextures(){
            if (!FontI || !ScreenObject) return;

            canvas* Canvas = ScreenObject->GetCanvas();
            if (!Canvas || !Canvas->Camera || !Canvas->Camera->ActiveWindow) return;
            
            SDL_Renderer* renderer = Canvas->Camera->ActiveWindow->SDLrenderer;
            if (!renderer) return;

            SDL_Surface* Surface = TTF_RenderText_Solid(FontI->GetTTFFont(), content.c_str(), content.length(), {255, 255, 255, 255});
            if (!Surface) return;
            
            SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, Surface);
            SDL_DestroySurface(Surface);
            
            if (Texture) SDL_DestroyTexture(Texture);
            Texture = newTexture;
        }

        void Text::Init(){
            // Don't create texture yet - wait for font to be set and SetText() to be called
        }

        void Text::Draw(window* Window){
            if (Window && Window->SDLrenderer && ScreenObject && ScreenObject->GetCanvas() && Texture){
                //get texture vectors
                float w, h;
                SDL_GetTextureSize(Texture, &w, &h);

                camera* ActiveCamera = ScreenObject->GetCanvas()->Camera;
                
                if (!ActiveCamera) return;

                //scale the rect
                float zoom = ActiveCamera->Zoom;
                float scaledH = h * ScreenObject->UITransform->GetWorldScale().y;
                float scaledW = w * ScreenObject->UITransform->GetWorldScale().x;

                SDL_FRect dst = {ScreenObject->UITransform->GetWorldPosition().x - scaledW / 2.0f , 
                    ScreenObject->UITransform->GetWorldPosition().y - scaledH / 2.0f, scaledW, scaledH };

                //set the texture colors
                SDL_SetTextureColorMod(Texture, Color.r, Color.g, Color.b);
                SDL_SetTextureAlphaMod(Texture, Color.a);

                SDL_FlipMode Flip = SDL_FLIP_NONE;
                
                //render it rotated as necessary
                SDL_RenderTextureRotated(
                    Window->SDLrenderer,
                    Texture,
                    NULL,   // src rect (whole texture)
                    &dst,   // dst rect
                    ScreenObject->UITransform->GetWorldRotation(),
                    NULL,   // center (NULL = center of dst)
                    Flip
                );
            }
        }

        void Text::OnDestroy(){
            if (Texture)
                SDL_DestroyTexture(Texture);
        }

    }
}