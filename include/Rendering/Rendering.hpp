#pragma once

#include <string>
#include <filesystem>
#include <SDL3_image/SDL_image.h>
#include "GlobalTypes.hpp"

namespace obj {
    enum class rendererBackend {
        SDL,    // Default 0
    };

    void Render();

    struct vector2;
    struct color;
    struct window;
    struct sprite;
    
    // Abstract texture class
    struct texture {
        virtual vector2 GetSize() = 0;
        virtual void SetTextureColorMod(const color& Color) = 0;
        virtual ~texture() = default;
    };

    // Abstract renderer struct
    struct renderer{
        virtual ~renderer() = default;
        virtual SDL_WindowFlags GetWindowCreationFlag() = 0;
        virtual void OnWindowCreation(window* Window){std::cout << "Created Window" << "\n";};
        virtual void Clear(window* Window) = 0;
        virtual void SetResolution(window* Window, const vector2 Size) = 0;
        virtual vector2 GetResolution(window* Window) = 0;
        virtual vector2 RenderCoordinatesFromWindow(window* Window, vector2 WindowPos) = 0;
        virtual void Present(window* Window) = 0;
        virtual void SetDrawColor(window* Window, const color& Color) = 0;
        virtual void FillRect(window* Window, const rect& Rect) = 0;
        virtual void DrawLine(window* Window, const vector2& A, const vector2& B) = 0;
        virtual void DrawTextureRotated(window* Window, texture* texture, const rect dstRect, float angle, const pivot& Pivot, FlipMode flip, const rect* srcRect = nullptr) = 0;
        virtual void DrawSpriteRotated(window* Window, sprite* Sprite, vector2 Position, vector2 Scale, float angle, const pivot& Pivot, FlipMode flip) = 0;
        virtual texture* CreateTextureFromSurface(window* Window, SDL_Surface* Surface) = 0;

        SDL_Surface* LoadSurfaceFromImage(const std::string& ImagePath){
            if (std::filesystem::exists(ImagePath)){
                SDL_Surface* SDLsurface = IMG_Load(ImagePath.c_str());

                return SDLsurface;
            }else{
                std::cout << "Image not Found:" << '"'<< ImagePath << '"' << "\n";
                return nullptr;
            }
        }
    };

    namespace Internal{
        extern renderer* Renderer;
    } //Internal
}