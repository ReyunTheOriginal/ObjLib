#include "Time.hpp"

namespace obj{
    namespace Internal{
        void time::Update(){CalculateDeltatime();}

        void time::CalculateDeltatime(){
            last = now;
            now = SDL_GetPerformanceCounter();
            DeltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
        }
    }//Internal
    Internal::time Time;
}//obl