#pragma once

#include <string>

struct TTF_Font;

namespace obj{
    struct font{
        private:
        TTF_Font* TTFFont = nullptr;
        int Size = 64;
        int ID = 0;

        friend font* CreateFont(std::string FontPath, float Size);

        public:

        void ChangeSize(float newSize);

        int GetID(){return ID;}

        TTF_Font* GetTTFFont(){return TTFFont;}

        ~font();
    };

    font* CreateFont(std::string FontPath, float Size = 64);
}