#include "Font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>
#include "GlobalLists.hpp"

namespace obj{
    font::~font(){
        if (TTFFont) TTF_CloseFont(TTFFont);
    }

    void font::ChangeSize(float newSize){
        if (newSize == Size) return;
        
        if (TTFFont)TTF_SetFontSize(TTFFont, newSize);
        Size = newSize;
    }

    font* CreateFont(std::string FontPath, float Size){
        if (std::filesystem::exists(FontPath)){
            font* newFont = new font();

            newFont->TTFFont = TTF_OpenFont(FontPath.c_str(), (int)Size);
            newFont->Size = Size;

            Internal::GlobalFonts.push_back(newFont);
            newFont->ID = Internal::Obj_ID;
            Internal::Obj_ID++;

            return newFont;
        }else{
            std::cout << "Font not Found:" << '"'<< FontPath << '"' << "\n";
            return nullptr;
        }
    }
}