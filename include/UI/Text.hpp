#pragma once

#include <string>
#include "UI/ScreenComponent.hpp"
#include <unordered_map>
#include "Font.hpp"

#include "GlobalTypes.hpp"

struct SDL_Texture;
struct SDL_Renderer;
struct TTF_Font;

namespace obj{
    namespace UI{
        struct Text : Internal::screenComponent{
            private:
            std::string content = "Hello, World!";
            SDL_Texture* Texture = nullptr;
            Font* FontI = nullptr;

            public:
            void SetText(std::string newText);
            std::string GetText(){return content;}

            color Color = {255,255,255,255};

            SDL_Texture* GetTexture(){
                return Texture;
            }

            void SetFont(Font* font){
                FontI = font;
                RecreateTextures();
            }
            Font* GetFont(){return FontI;}

            void RecreateTextures();
            
            void Init() override;
            void Draw(window* Window) override;
            void OnDestroy() override;
            
        };
    }
}