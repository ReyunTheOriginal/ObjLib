#pragma once

#include <string>
#include "UI/ScreenComponent.hpp"
#include <unordered_map>
#include "Font.hpp"

#include "GlobalTypes.hpp"

struct SDL_Renderer;
struct TTF_Font;

namespace obj{
    struct texture;
    namespace UI{
        struct text : screenComponent{
            private:
            std::string content = "Hello, World!";
            texture* Texture = nullptr;
            font* FontI = nullptr;

            public:
            void SetText(std::string newText);
            std::string GetText(){return content;}
            pivot Pivot = pivot::Center;

            color Color = {255,255,255,255};

            texture* GetTexture(){
                return Texture;
            }

            void SetFont(font* font){
                FontI = font;
                RecreateTextures();
            }
            font* GetFont(){return FontI;}

            void RecreateTextures();
            
            void Init() override;
            void Draw(window* Window) override;
            void OnDestroy() override;
            
        };
    }
}