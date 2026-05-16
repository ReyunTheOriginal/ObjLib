#include "UI/Text.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "UI/UIBase.hpp"
#include "Camera/Camera.hpp"
#include "Rendering/Rendering.hpp"

namespace obj{
    namespace UI{

        void text::SetText(std::string newText){
            if (newText == content) return;
            content = newText;
            RecreateTextures();
        }

        void text::RecreateTextures(){
            if (!FontI || !GetScreenObject()) return;

            canvas* Canvas = GetScreenObject()->GetCanvas();
            if (!Canvas || !Canvas->GetCamera() || !Canvas->GetCamera()->GetWindow()) return;

            SDL_Surface* Surface = TTF_RenderText_Solid(FontI->GetTTFFont(), content.c_str(), content.length(), {255, 255, 255, 255});
            if (!Surface) return;
            
            texture* newTexture = ::obj::Internal::Renderer->CreateTextureFromSurface(Canvas->GetCamera()->GetWindow(), Surface);
            SDL_DestroySurface(Surface);
            
            if (Texture) delete Texture;
            Texture = newTexture;
        }

        void text::Init(){
            // Don't create texture yet - wait for font to be set and SetText() to be called
        }

        void text::Draw(window* Window){
            if (Window && GetScreenObject() && GetScreenObject()->GetCanvas() && Texture){
                //get texture vectors
                vector2 texsize = Texture->GetSize();

                camera* ActiveCamera = GetScreenObject()->GetCanvas()->GetCamera();
                
                if (!ActiveCamera) return;

                //scale the rect
                float scaledW = texsize.x * GetScreenObject()->UITransform->GetScreenScale().x;
                float scaledH = texsize.y * GetScreenObject()->UITransform->GetScreenScale().y;

                SDL_FRect dst = {GetScreenObject()->UITransform->GetScreenPosition().x - scaledW * Pivot.Point.x , 
                    GetScreenObject()->UITransform->GetScreenPosition().y - scaledH * Pivot.Point.y, scaledW, scaledH };

                //set the texture colors
                Texture->SetTextureColorMod(Color);

                SDL_FlipMode Flip = SDL_FLIP_NONE;
                
                //render it rotated as necessary
                ::obj::Internal::Renderer->DrawTextureRotated(
                    Window,
                    Texture,
                    dst,   // dst rect
                    GetScreenObject()->UITransform->GetScreenRotation(),
                    {0.5,0.5},   // center (NULL = center of dst)
                    Flip
                );
            }
        }

        void text::OnDestroy(){
            if (Texture)
                delete Texture;
        }

    }
}