#pragma once

#include <string>

struct TTF_Font;

namespace obj{
    struct Font{
        private:
        TTF_Font* TTFFont = nullptr;
        int Size = 64;

        friend Font* CreateFont(std::string FontPath, float Size);

        public:

        void ChangeSize(float newSize);

        TTF_Font* GetTTFFont(){return TTFFont;}

        ~Font();
    };

    Font* CreateFont(std::string FontPath, float Size = 64);
}