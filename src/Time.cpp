#include "Core.hpp"

namespace obl{
    namespace Internal{
        void time::Update(){
            CalculateDeltatime();
        }

        void time::CalculateDeltatime(){
            last = now;
            now = SDL_GetPerformanceCounter();
            DeltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
        }
    }//Internal
    Internal::time Time;
}//obl