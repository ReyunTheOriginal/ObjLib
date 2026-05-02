#include "Font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>

namespace obj{
    Font::~Font(){
        if (TTFFont) TTF_CloseFont(TTFFont);
    }

    void Font::ChangeSize(float newSize){
        if (newSize == Size) return;
        
        if (TTFFont)TTF_SetFontSize(TTFFont, newSize);
        Size = newSize;
    }

    Font* CreateFont(std::string FontPath, float Size){
        if (std::filesystem::exists(FontPath)){
            Font* newFont = new Font();

            newFont->TTFFont = TTF_OpenFont(FontPath.c_str(), (int)Size);
            newFont->Size = Size;

            return newFont;
        }else{
            std::cout << "Font not Found:" << '"'<< FontPath << '"' << "\n";
            return nullptr;
        }
    }
}