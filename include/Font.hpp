#pragma once

#include <string>

struct TTF_Font;

namespace obj{
    struct font{
        private:
        TTF_Font* TTFFont = nullptr;
        int Size = 64;

        friend font* CreateFont(std::string FontPath, float Size);

        public:

        void ChangeSize(float newSize);

        TTF_Font* GetTTFFont(){return TTFFont;}

        ~font();
    };

    font* CreateFont(std::string FontPath, float Size = 64);
}