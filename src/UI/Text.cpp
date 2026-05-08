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
            if (!Canvas || !Canvas->GetCamera() || !Canvas->GetCamera()->GetWindow()) return;
            
            SDL_Renderer* renderer = Canvas->GetCamera()->GetWindow()->GetSDLRenderer();
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
            if (Window && Window->GetSDLRenderer() && ScreenObject && ScreenObject->GetCanvas() && Texture){
                //get texture vectors
                float w, h;
                SDL_GetTextureSize(Texture, &w, &h);

                camera* ActiveCamera = ScreenObject->GetCanvas()->GetCamera();
                
                if (!ActiveCamera) return;

                //scale the rect
                float zoom = ActiveCamera->Zoom;
                float scaledH = h * ScreenObject->UITransform->GetScreenScale().y;
                float scaledW = w * ScreenObject->UITransform->GetScreenScale().x;

                SDL_FRect dst = {ScreenObject->UITransform->GetScreenPosition().x - scaledW / 2.0f , 
                    ScreenObject->UITransform->GetScreenPosition().y - scaledH / 2.0f, scaledW, scaledH };

                //set the texture colors
                SDL_SetTextureColorMod(Texture, Color.r, Color.g, Color.b);
                SDL_SetTextureAlphaMod(Texture, Color.a);

                SDL_FlipMode Flip = SDL_FLIP_NONE;
                
                //render it rotated as necessary
                SDL_RenderTextureRotated(
                    Window->GetSDLRenderer(),
                    Texture,
                    NULL,   // src rect (whole texture)
                    &dst,   // dst rect
                    ScreenObject->UITransform->GetScreenRotation(),
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