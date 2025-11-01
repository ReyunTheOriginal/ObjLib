#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Objects.hpp"
#include "GlobalTypes.hpp"
#include "HelperFunctions.hpp"
#include "Math.hpp"

namespace obl{
    namespace components{
        struct renderLayer : Internal::component{
            int RenderOrder = 0;
            int Layer = 0;

            renderLayer(int order = 0, int layer = 0){
                Layer = layer;RenderOrder = order;
            };
            void Run() override;
        };
        
        struct SpriteRenderer : Internal::component{
            private:
            SDL_Texture* Texture = nullptr;
            std::string Sprite = "Triangle";

            void CreateTexture();

            public:
            bool Drawn = true;
            color Color = {255,0,0,255};

            bool FlippedX = false;
            bool FlippedY = false;

            std::string GetSprite();
            void SetSprite(std::string NewSprite);

            void Run() override;
            void Draw() override;

            void Init() override{
                if (GameObject->GetComponent<renderLayer>() == nullptr) GameObject->AddComponent<renderLayer>();
            }

            ~SpriteRenderer(){
                if (Texture && SDL_WasInit(SDL_INIT_EVERYTHING)) SDL_DestroyTexture(Texture);
            }
        };
        
    }
}