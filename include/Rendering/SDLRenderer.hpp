#pragma once

#include "Rendering/Rendering.hpp"
#include <SDL3/SDL.h>
#include <unordered_map>

namespace obj {
    

    // Concrete SDL texture
    struct sdlTexture : texture {
        SDL_Texture* tex = nullptr;
        vector2 GetSize() override{
            float x,y;
            SDL_GetTextureSize(tex, &x, &y);
            return {x,y};
        }
        void SetTextureColorMod(const color& Color) override;
        sdlTexture(SDL_Texture* t) : tex(t) {}
        ~sdlTexture() override { if (tex) SDL_DestroyTexture(tex); }
    };

    // Concrete SDL renderer
    struct sdlRenderer : renderer {
    private:
        std::unordered_map<window*, SDL_Renderer*> SDLRenderers;
        std::unordered_map<window*, vector2> Resolutions;

    public:
        sdlRenderer() {}
        ~sdlRenderer() override;
        void OnWindowCreation(window* Window) override;
        SDL_WindowFlags GetWindowCreationFlag() {return SDL_WINDOW_RESIZABLE;};
        void Clear(window* Window) override;
        void SetResolution(window* Window, const vector2 Size) override;
        vector2 GetResolution(window* Window) override{return Resolutions[Window];}
        void Present(window* Window) override;
        void SetDrawColor(window* Window, const color& Color) override;
        vector2 RenderCoordinatesFromWindow(window* Window, vector2 WindowPos) override;
        void FillRect(window* Window, const rect& Rect) override;
        void DrawLine(window* Window, const vector2& A, const vector2& B) override;
        void DrawTextureRotated(window* Window, texture* texture, const rect dstRect, float angle, const pivot& Pivot, FlipMode flip, const rect* srcRect = nullptr) override;
        void DrawSpriteRotated(window* Window, sprite* Sprite, vector2 Position, vector2 Scale, float angle, const pivot& Pivot, FlipMode flip) override;
        texture* CreateTextureFromSurface(window* Window, SDL_Surface* Surface) override;
    };
}