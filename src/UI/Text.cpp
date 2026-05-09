#include "UI/Text.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "UI/UIBase.hpp"
#include "Camera/Camera.hpp"

namespace obj{
    namespace UI{

        void Text::SetText(std::string newText){
            if (newText == content) return;
            content = newText;
            RecreateTextures();
        }

        void Text::RecreateTextures(){
            if (!FontI || !GetScreenObject()) return;

            canvas* Canvas = GetScreenObject()->GetCanvas();
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
            if (Window && Window->GetSDLRenderer() && GetScreenObject() && GetScreenObject()->GetCanvas() && Texture){
                //get texture vectors
                float w, h;
                SDL_GetTextureSize(Texture, &w, &h);

                camera* ActiveCamera = GetScreenObject()->GetCanvas()->GetCamera();
                
                if (!ActiveCamera) return;

                //scale the rect
                float zoom = ActiveCamera->Zoom;
                float scaledH = h * GetScreenObject()->UITransform->GetScreenScale().y;
                float scaledW = w * GetScreenObject()->UITransform->GetScreenScale().x;

                SDL_FRect dst = {GetScreenObject()->UITransform->GetScreenPosition().x - scaledW * Pivot.Point.x , 
                    GetScreenObject()->UITransform->GetScreenPosition().y - scaledH * Pivot.Point.y, scaledW, scaledH };

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
                    GetScreenObject()->UITransform->GetScreenRotation(),
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