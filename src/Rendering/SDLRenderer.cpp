#include "Rendering/SDLRenderer.hpp"
#include "GlobalTypes.hpp"
#include <SDL3_image/SDL_image.h>
#include "Window.hpp"
#include <filesystem>
#include "Sprite.hpp"

namespace obj{

    void sdlTexture::SetTextureColorMod(const color& Color){
        SDL_SetTextureColorMod(tex, Color.r, Color.g, Color.b);
        SDL_SetTextureAlphaMod(tex, Color.a);
    }


    void sdlRenderer::SetDrawColor(window* Window, const color& Color){ 
        SDL_SetRenderDrawColor(SDLRenderers[Window], (Uint8)Color.r, (Uint8)Color.g, (Uint8)Color.b, (Uint8)Color.a); 
    }

    sdlRenderer::~sdlRenderer(){
        for (auto& renderer : SDLRenderers)
            if (renderer.second) SDL_DestroyRenderer(renderer.second);
        
    }

    void sdlRenderer::OnWindowCreation(window* Window){
        SDLRenderers[Window] = SDL_CreateRenderer(Window->GetSDLWindow(), NULL);
    }

    void sdlRenderer::SetResolution(window* Window, const vector2 Size){
        SDL_SetRenderLogicalPresentation(SDLRenderers[Window], Size.x, Size.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        Resolutions[Window] = Size;
    }

    vector2 sdlRenderer::RenderCoordinatesFromWindow(window* Window, vector2 WindowPos){
         float logicalX, logicalY;
        SDL_RenderCoordinatesFromWindow(
            SDLRenderers[Window],
            WindowPos.x, WindowPos.y,
            &logicalX, &logicalY
        );
        return {logicalX, logicalY};
    }

    void sdlRenderer::Clear(window* Window){
        SDL_RenderClear(SDLRenderers[Window]); 
    }

    void sdlRenderer::Present(window* Window){
        SDL_RenderPresent(SDLRenderers[Window]); 
    }

    void sdlRenderer::FillRect(window* Window, const rect& Rect){
        SDL_RenderFillRect(SDLRenderers[Window], &Rect); 
    }

    void sdlRenderer::DrawLine(window* Window, const vector2& A, const vector2& B){ 
        SDL_RenderLine(SDLRenderers[Window], A.x, A.y, B.x, B.y); 
    }

    void sdlRenderer::DrawTextureRotated(window* Window, texture* texture, const rect dstRect, float angle, const pivot& Pivot, FlipMode flip, const rect* srcRect) {
        auto sdlTex = static_cast<sdlTexture*>(texture)->tex;
        SDL_FPoint point = Pivot.Point;
        SDL_RenderTextureRotated(SDLRenderers[Window], sdlTex, srcRect, &dstRect, angle, &point, flip);
    }

    void sdlRenderer::DrawSpriteRotated(window* Window, sprite* Sprite, vector2 Position = {0,0}, vector2 Scale = {0,0}, float angle = 0, const pivot& Pivot = {0.5, 0.5}, FlipMode flip = SDL_FLIP_NONE){
        auto sdlTex = static_cast<sdlTexture*>(Sprite->Textures[Window])->tex;
        SDL_FRect dst = {Position.x - Scale.x * Pivot.Point.x , 
                    Position.y - Scale.y * Pivot.Point.y, Scale.x, Scale.y };

        SDL_RenderTextureRotated(SDLRenderers[Window], sdlTex, NULL, &dst, angle, NULL, flip);
    }

    texture* sdlRenderer::CreateTextureFromSurface(window* Window, SDL_Surface* Surface){
       return new sdlTexture(SDL_CreateTextureFromSurface(SDLRenderers[Window], Surface));
    }
}